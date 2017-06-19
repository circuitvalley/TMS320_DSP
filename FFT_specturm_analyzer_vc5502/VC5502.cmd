/****************************************************************************/
/*  C5502.cmd                                                               */
/*  Copyright (c) 2010  Texas Instruments Incorporated                      */
/*  Author: Rafael de Souza                                                 */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on a C5502.              */
/*                 Use it as a guideline.  You will want to                 */
/*                 change the memory layout to match your specific          */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/

MEMORY
{
    MMR:     o = 0x000000  l = 0x0000c0  /* 192B Memory Mapped Registers */
    DARAM0:  o = 0x0000C0  l = 0x00FF40  /* 8kB Dual Access RAM 0 */
  //  DARAM1:  o = 0x002000  l = 0x002000  /* 8kB Dual Access RAM 1 */
  //  DARAM2:  o = 0x004000  l = 0x002000  /* 8kB Dual Access RAM 2 */
 //   DARAM3:  o = 0x006000  l = 0x002000  /* 8kB Dual Access RAM 3 */
 //   DARAM4:  o = 0x008000  l = 0x002000  /* 8kB Dual Access RAM 4 */
 //   DARAM5:  o = 0x00A000  l = 0x002000  /* 8kB Dual Access RAM 5 */
 //   DARAM6:  o = 0x00C000  l = 0x002000  /* 8kB Dual Access RAM 6 */
//    DARAM7:  o = 0x00E000  l = 0x002000  /* 8kB Dual Access RAM 7 */
  
    CE0:     o = 0x010000  l = 0x3F0000  /* 4MB CE0 external memory space */
    CE1:     o = 0x400000  l = 0x400000  /* 4MB CE1 external memory space */
    CE2:     o = 0x800000  l = 0x400000  /* 4MB CE2 external memory space */
    CE3:     o = 0xC00000  l = 0x3F8000  /* 4MB CE3 external memory space */
    ROM:      o = 0xFF8000  l = 0x007F00  /* 32kB ROM (MPNMC=0) or CE3 (MPNMC=1) */
    VECS:     o = 0xFFFF00  l = 0x000100  /* reset vector */
}

SECTIONS
{
    vectors (NOLOAD)
    vector 		   >  VECS  /* If MPNMC = 1, remove the NOLOAD directive */
    .cinit         >  CE0
    .text          >  DARAM0
    .stack         >  DARAM0
    .sysstack      >  DARAM0
    .sysmem        >  DARAM0
    .data          >  DARAM0
    .cio           >  DARAM0
    .bss           >  DARAM0
    .const         >  DARAM0
}

-l C:\Users\Gaurav\Documents\TI_workspace\tms320vc5502.cmd
