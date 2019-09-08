/*
 * Copyright (C) 2014-2019 Daniel Rossier <daniel.rossier@heig-vd.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

@
@ entry points and handlers for exceptions and interrupts
@
@ based on explanations in "Building Bare Metal ARM Systems with GNU"
@ http://www.state-machine.com/arm/Building_bare-metal_ARM_with_GNU.pdf
@ and on U-Boot implementation

#include <linkage.h>

#include <thread.h>

#include <asm-offsets.h>

#include <asm/syscall.h>
#include <asm/processor.h>

/* use the special section (.vectors.text), to enable fine-tuning
 * of the placement of this section inside the linker script
 */
.section ".vectors.text", "ax"

.globl _vectors
.globl ret_from_fork
.globl __get_syscall_args_ext
.global __call_sig_handler

.extern irq_handle
.extern syscall_handle
.extern dumpstack
.extern __prefetch_abort
.extern __data_abort
.extern do_exit
.extern schedule_isr
.extern __check_ptrace_syscall
.extern sig_check
.extern __stack_alignment_fault

.extern _start

ENTRY(_vectors)
		    b   _start @ reset
            ldr pc, _undefined_instruction
            ldr pc, _syscall_interrupt
            ldr pc, _prefetch_abort
            ldr pc, _data_abort
            ldr pc, _not_used
            ldr pc, _irq
            ldr pc, _fiq

_undefined_instruction: .word undefined_instruction
_syscall_interrupt:     .word syscall_interrupt
_prefetch_abort:        .word prefetch_abort
_data_abort:            .word data_abort
_not_used:              .word not_used
_irq:                   .word irq
_fiq:                   .word fiq
_pad:                   .word 0x12345678 @ now 16*4=64

@ exception handlers
.align  5
undefined_instruction:
    b  undefined_instruction


@ Prepare to call a handler associated to a pending signal
@ r0 contains the reference to the sigaction_t structure related to the signal to be processed.

__prepare_sig_handler:

	ldr		r11, [r0, #OFFSET_SYS_STACK_TOP]
	sub		r11, r11, #SVC_STACK_FRAME_SIZE

	sub 	sp, sp, #4			@ Make sure we stay 8-bytes aligned to be compliant with EABI

	@ Build a new stack frame based on the current
	sub		sp, sp, #SVC_STACK_FRAME_SIZE

	@ Start the copy - r11 is the dest, r12 is the src
	mov		r12, sp				@ Base of the new stack frame

	ldmia	r11!, {r1-r10}		@ from sp_usr to r7
	stmia	r12!, {r1-r10}
	ldmia	r11!, {r1-r9}		@ from r8 to pc (last register)
	stmia	r12!, {r1-r9}

	@ Make sure the spsr is with Thumb de-activated to perform normal execution of the handler
	ldr		r1, [sp, #15*4]    	@ Load SPSR
	bic		r1, r1, #0x20		@ Clear Thumb-mode
	str		r1, [sp, #15*4]		@ Save the new SPSR

	@ Set the argument (signum, handler) to r0 & r1
	ldr		r1, [r0, #OFFSET_SYS_SIGNUM]
	str		r1, [sp, #8]

	ldr 	r0, [r0, #OFFSET_SYS_SA]
	ldr		r1, [r0, #OFFSET_SA_HANDLER]
	str		r1, [sp, #12]

	ldr		r1, [r0, #OFFSET_SA_RESTORER]
	str		r1, [sp, #SVC_STACK_FRAME_SIZE - 4]		@ Set the handler to the PC

	@ Preserve our new stack base
	str		sp, [sp, #16*4]

	mov		pc, lr				@ Back to the caller

.macro check_pending_signal
	@ Is there any pending signals for this process?
	bl		sig_check

	cmp		r0, #0
	beq		1f

	@ Configure a stack frame to manage the user handler
	bl		__prepare_sig_handler
1:

.endm

@ About the stack frame...
@ Currently, the stack frame has the following layout:
@ { pc (lr), lr, sp, spsr, r12, r11, ..., r0, sp_usr, lr_usr } from the higher address to the lower
@ The (twice) lr register at the beginning is probably redundant, but this will be checked in the future.

@ IRQs are off
@ ARM EABI: the syscall nr is stored in r7
.align  5
syscall_interrupt:

	@ Build the stack frame to store registers

	str		sp, [sp, #-12]
	str		lr, [sp, #-8]
	str		lr, [sp, #-4]

	sub		sp, sp, #16			@ Point on spsr

	stmfd 	sp!, {r0-r12}  		@ Store registers

    mrs 	lr, spsr            @ Get spsr

	str		lr, [sp, #13*4]		@ Store spsr

    sub		sp, sp, #8
    stmia 	sp, {sp, lr}^		@ Store sp_usr and lr_usr

	@ Give a chance to a ptrace tracer to monitor us (before the syscall)
	stmfd 	sp!, {r0-r4}
	bl		__check_ptrace_syscall
	ldmfd 	sp!, {r0-r4}

	@ Keep the stack aligned on 8-byte to stay compliant with EABI
	tst		sp, #7				@ already 8-byte aligned?
	movne	lr, #0
	strne	lr, [sp, #-4]		@ not aligned, store a padding location
	subne	sp, sp, #4

	moveq 	lr, #4
	streq	lr, [sp, #-4]
	streq	lr, [sp, #-8]
	subeq	sp, sp, #8

	cpsie   i 				 	@ Re-enable interrupts
    bl 		syscall_handle
    cpsid	i					@ Re-disable interrupts to be safe in regs manipulation

	@ Restore the stack situation according to previous alignment
	ldr		lr, [sp]
	cmp		lr, #4
	addeq	sp, sp, #8
	addne	sp, sp, #4

	@ Check if sigreturn has been called. In this case, we
	@ clean the stack frame which has been used to manage the user handler.
	cmp		r7, #SYSCALL_SIGRETURN
	bne		__no_sigreturn

	@ Clean the stack frame used for signal handling
	add		sp, sp, #SVC_STACK_FRAME_SIZE	@ First, clean the current stack frame used by sigreturn

	add		sp, sp, #SVC_STACK_FRAME_SIZE	@ Finally, clean the stack frame used to manage the signal handling
	add		sp, sp, #4			@ Restore 4 bytes used to keep a 8-byte alignment

__no_sigreturn:

	@ Give a chance to a ptrace tracer to monitor us (after the syscall)
	stmfd 	sp!, {r0-r4}
	bl	__check_ptrace_syscall
	ldmfd 	sp!, {r0-r4}

@ The stack pointer is at 14 * 4 bytes under its top.
__ret_from_fork:

	@ Store the return value on the stack frame
	cmp		r7, #SYSCALL_SIGRETURN
	strne	r0, [sp, #8]		@ Do not corrupt r0 if we come from a SVC context

#ifdef CONFIG_IPC_SIGNAL
	@ Is there any pending signals for this process?

	check_pending_signal
#endif /* CONFIG_IPC_SIGNAL */

	ldr		r4, [sp, #15*4]
	and		r4, r4, #MODE_MASK

	cmp		r4, #USR_MODE

	@ Restoring user mode registers (sp_usr, lr_usr)
	ldmeqia	sp, {sp, lr}^		@ Do not modify sp_usr and lr_usr if we did not come from USR mode

	add		sp, sp, #8

    ldmfd 	sp!, {r0-r12}

    ldr 	lr, [sp], #4        @ get the saved spsr and adjust the stack pointer
    msr		spsr, lr

    ldmia 	sp, {sp, lr, pc}^

@ Used at entry point of a fork'd process (setting the return value to 0)
ret_from_fork:
	mov		r0, #0
	b		__ret_from_fork


@ Get the additional arguments linked to the syscall.
@ The ABI convention is described in crt0.S of the libc.
@ r7 contains the syscall number
@ r10 contains the address of the errno variable
__get_syscall_args_ext:

	str		r7, [r0]
	str		r10, [r1]

	mov 	pc, lr

.align  5
prefetch_abort:

	mrc	p15, 0, r0, c6, c0, 2		@ get IFAR
	mrc	p15, 0, r1, c5, c0, 1		@ get IFSR

    b __prefetch_abort

.align  5
data_abort:

	mrc	p15, 0, r1, c5, c0, 0		@ get FSR
	mrc	p15, 0, r0, c6, c0, 0		@ get FAR

    b __data_abort

.align  5
not_used:
    b not_used

.align  5
irq:

    @ IRQ mode (lr_irq, sp_irq, cpsr_irq)

    @ Store original r0, and lr_irq in the IRQ stack sp = sp_irq
    @ sp_irq is a simple array declared in arch/arm/setup.c (so address can be ascending)

    str 	r0, [sp]		@ original r0
    str 	lr, [sp, #4]	@ lr_irq

    mrs 	r0, spsr 		@ to preserve irq bit
    str		r0, [sp, #8]	@ spsr_irq

    mov		r0, sp			@ to maintain a reference on sp_irq; r0 will not be affected after mode switch

    @ Now switch back to SVC. IRQs are disabled
    mrs		lr, cpsr

    bic		lr, lr, #MODE_MASK
    orr		lr, lr, #SVC_MODE

    @ switch to SVC
    msr		cpsr, lr

    @ --- SVC mode from now on ---

    @ On the stack, we preserve {spsr, sp, lr, pc} from the current sp location
    str		lr, [sp, #-8]   @ current lr

    @ preserve spsr for future IRQ enabling
    ldr		lr, [r0, #8]   	@ retrieve spsr_irq
    str		lr, [sp, #-16] 	@ location of spsr_irq (hence cpsr right before the interrupt)

	@ Get original lr (as before irq)

	ldr 	lr, [r0, #4]	@ retrieve lr_irq
	sub 	lr, lr, #4		@ Adjust the lr since it is automatically set from pc (in advance of 2 instructions due to the pipeline)
	str 	lr, [sp, #-4]   @ future pc at exit

	str 	sp, [sp, #-12]  @ preserve sp

	sub 	sp, sp, #16	    @ adjust for the call to IRQ handler, sp points to spsr_irq

	ldr 	r0, [r0]		@ original r0

    stmfd 	sp!, {r0-r12}
	
	sub		sp, sp, #8
	stmia	sp, {sp, lr}^	@ May be from USR, or not (will not be restored if not coming from USR)

	@ At this point, the stack frame is 19*4 bytes aligned, which is not compliant with ARM-EABI
	@ Keep the stack aligned on 8-byte to stay compliant with EABI considering that the stack cannoot be 8-byte aligned at
	@ the entrance of the interrupt routine.

	tst		sp, #7				@ already 8-byte aligned? (storing on the stack is multiple of 4 bytes anyway)
	movne	lr, #0
	strne	lr, [sp, #-4]		@ not aligned, store a padding location
	subne	sp, sp, #4

	moveq 	lr, #4				@ Store value 4 in two additional stack cells. Used for unstacking policy.
	streq	lr, [sp, #-4]
	streq	lr, [sp, #-8]
	subeq	sp, sp, #8

	@ Make sure r0 refers to the base of the stack frame
	mov		r0, sp
    bl 		irq_handle

	@ Give the opportunity to pending softirq to be executed (IRQs off, but can be (synchronously) preempted)
    bl 		do_softirq

	@ Restore the stack situation according to previous alignment
	ldr		lr, [sp]			@ Determine how many bytes to unstack according to previous alignment
	cmp		lr, #4
	addeq	sp, sp, #8
	addne	sp, sp, #4

#ifdef CONFIG_IPC_SIGNAL
	@ Is there any pending signals for this process?

	check_pending_signal
#endif /* CONFIG_IPC_SIGNAL */

	ldr		r4, [sp, #15*4]
	and		r4, r4, #MODE_MASK

	cmp		r4, #USR_MODE

	@ Restoring user mode registers (sp_usr, lr_usr)
	ldmeqia	sp, {sp, lr}^	@ Do not modify sp_usr and lr_usr if we did not come from USR mode

	add		sp, sp, #8

    ldmfd 	sp!, {r0-r12}

    ldr 	lr, [sp], #4        @ get the saved spsr and adjust the stack pointer
    msr		spsr, lr

    @ Now, we retrieve the final registers, sp will be adjusted automatically
    ldmia 	sp, {sp, lr, pc}^

.align  5
fiq:
    b  fiq
