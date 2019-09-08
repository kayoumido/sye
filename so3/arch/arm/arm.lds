/*
 * Linker script for so3 kernel 
 */

OUTPUT_FORMAT(elf32-littlearm)
OUTPUT_ARCH(arm)
ENTRY(_start)

/* The sizes of the stacks used by the application. */
 
SVC_STACK_SIZE = 8192 + 10 * 32 * 1024;  /* Initial system stack + 320 KB stack available for max. 10 kernel threads */

HEAP_SIZE = 512 * 1024;     /* 512 KB dedicated to kernel heap */
TABLE_L1_SIZE = 16 * 1024;   /* 16 KB 1st-level page table */ 

/*
 * The system (1st-level) page table is located at 0xc000'4000 and occupies 16 KB (4096 32-bit entries)
 */ 
 
 
SECTIONS
{
   
     . = ENTRY_ADDR + 0x8000;   
    
    __vectors_start = .;

    .vectors.text :
    {
        *(.vectors.text); 
    }
    
  	__vectors_end = .;
  
    . = ALIGN(16384);
    

    .head.text :
    {
        *(.head.text);
    }

    .text :
    {
        *(.text*) 
    }

    . = ALIGN(4096);
    .data  : 
    { 
        *(.data*) 
    }

    . = ALIGN(4096);
    __bss_start = .;

    .sbss (NOLOAD) : 
    { 
        *(.sbss*)
    }

    .bss (NOLOAD) : 
    { 
        *(.bss*) 
        . = ALIGN(4096); 
    }

    __bss_end = .;

	. = ALIGN(4096);
	.linker_list : {
		KEEP(*(SORT(.linker_list*)));
	}

    . = ALIGN(4096);

    .heap :
    {
      __heap_base_addr = . ;
     
      . += HEAP_SIZE;
    }

    . = ALIGN(4096);

    .stack : 
    {
        __stack_base = . ;
 
        . += SVC_STACK_SIZE;
        . = ALIGN (8);
        __stack_top = . ;
        
       }

    __end = .;
}
