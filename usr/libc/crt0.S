/*
 * Copyright (C) 2014-2017 Daniel Rossier <daniel.rossier@heig-vd.ch>
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

#include <syscall.h>

.extern __entryC

.data
.align 	4
.globl	errno

errno:
	.word	0x0

.text
.align  4

/* -------------------------------------------------------------
 * __start
 *	Initialize running a C program, by calling "main".
 * -------------------------------------------------------------
 */

	.globl	__start
__start:

	@ r0 points to the argument page

	bl		__entryC

	b		exit	 /* if we return from main, exit(return value) */

	nop

/* -------------------------------------------------------------
 * System call stubs:
 * - r0-r3 are used to store arguments
 * - r7 is used to store the syscall number
 * -------------------------------------------------------------
 */

.macro SYSCALLSTUB name, number
	.globl	\name
\name:

  stmfd sp!, {r7, r10, r11}

  mov	r7, #\number
  ldr	r10, =errno
  mov	r11, #0
  str	r11, [r10]

  swi	0

  ldmfd sp!, {r7, r10, r11}

  mov pc, lr

.endm

/* Syscalls stubs */
SYSCALLSTUB sys_halt, 			    syscallHalt
SYSCALLSTUB sys_write,			    syscallWrite
SYSCALLSTUB sys_read, 			    syscallRead
SYSCALLSTUB sys_exit, 			    syscallExit
SYSCALLSTUB sys_execve,			    syscallExecve
SYSCALLSTUB sys_waitpid,		    syscallWaitpid
SYSCALLSTUB sys_pause, 			    syscallPause
SYSCALLSTUB sys_fork, 			    syscallFork
SYSCALLSTUB sys_readdir, 		    syscallReaddir
SYSCALLSTUB sys_chdir, 			    syscallChdir
SYSCALLSTUB sys_getcwd, 		    syscallGetcwd
SYSCALLSTUB sys_creat, 			    syscallCreate
SYSCALLSTUB sys_unlink, 		    syscallUnlink
SYSCALLSTUB sys_open, 			    syscallOpen
SYSCALLSTUB sys_close, 			    syscallClose
SYSCALLSTUB sys_thread_create,  syscallThreadCreate
SYSCALLSTUB sys_thread_join, 	  syscallThreadJoin
SYSCALLSTUB sys_thread_exit, 	  syscallThreadExit
SYSCALLSTUB sys_thread_yield,   syscallThreadYield
SYSCALLSTUB sys_pipe, 			    syscallPipe
SYSCALLSTUB sys_ioctl,			    syscallIoctl
SYSCALLSTUB sys_mkdir,			    syscallMkdir
SYSCALLSTUB sys_stat,	 		      syscallStat
SYSCALLSTUB sys_dup,	 		      syscallDup
SYSCALLSTUB sys_dup2,	 		      syscallDup2
SYSCALLSTUB sys_renice,         syscallRenice
SYSCALLSTUB sys_sched_setparam, syscallSchedSetParam
SYSCALLSTUB sys_socket, 		    syscallSocket
SYSCALLSTUB sys_bind, 			    syscallBind
SYSCALLSTUB sys_listen, 		    syscallListen
SYSCALLSTUB sys_accept, 		    syscallAccept
SYSCALLSTUB sys_connect, 		    syscallConnect
SYSCALLSTUB sys_mmap, 			    syscallMmap
SYSCALLSTUB sys_hidestr,        syscallHidestr

SYSCALLSTUB sys_getpid,			    syscallGetpid

SYSCALLSTUB sys_gettimeofday,	  syscallGetTimeOfDay
SYSCALLSTUB sys_settimeofday,	  syscallSetTimeOfDay
SYSCALLSTUB sys_sbrk,			      syscallSbrk
SYSCALLSTUB sys_info,	    	    syscallSysinfo

SYSCALLSTUB sys_mutex_lock,	    syscallMutexLock
SYSCALLSTUB sys_mutex_unlock,	  syscallMutexUnlock

SYSCALLSTUB sys_sigreturn,		  syscallSigreturn

SYSCALLSTUB sys_nanosleep,		  syscallNanosleep


