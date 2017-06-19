;/*
; * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/ 
; * 
; * 
; *  Redistribution and use in source and binary forms, with or without 
; *  modification, are permitted provided that the following conditions 
; *  are met:
; *
; *    Redistributions of source code must retain the above copyright 
; *    notice, this list of conditions and the following disclaimer.
; *
; *    Redistributions in binary form must reproduce the above copyright
; *    notice, this list of conditions and the following disclaimer in the 
; *    documentation and/or other materials provided with the   
; *    distribution.
; *
; *    Neither the name of Texas Instruments Incorporated nor the names of
; *    its contributors may be used to endorse or promote products derived
; *    from this software without specific prior written permission.
; *
; *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
; *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
; *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
; *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
; *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
; *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
; *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
; *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
; *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
; *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
; *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
; *
;*/

;***********************************************************
; Version 3.00.00                                           
;***********************************************************
;*********************************************************************
; Function:    cbrev
; Processor:   C55xx
; Description: This function performs bit-reversal of complex data array x.
;              If x==y, in-place bit reversal is performed.
;
; Usage:    void cbrev (DATA *x, DATA *y, ushort n)
;
; Benchmarks:
;   Cycles:
;      x != y:  (2 * nx) + 33
;     nx =   8:  x=y:   79   x!=y:   49
;     nx =  16:  x=y:   98   x!=y:   65
;     nx =  32:  x=y:  124   x!=y:   97
;     nx =  64:  x=y:  198   x!=y:  161
;     nx = 128:  x=y:  277   x!=y:  289
;     nx = 256:  x=y:  512   x!=y:  545
;     nx = 512:  x=y:  880   x!=y: 1057
;     nx =1024:  x=y: 1739   x!=y: 2080
;   Code Size (in bytes):     214
;
; History:
;   Craig Leeds 05/25/2012: Set BK47 when using AR4 as circular buffer
;****************************************************************

		.mmregs  
		.cpl_on   
                .noremark  5684  ; BRC1 not modified by MMR within Repeat Blocks
        .global _cbrev   
	.text
_cbrev:

;//-----------------------------------------------------------------------------
;// Context Save
;//-----------------------------------------------------------------------------

        PSH     mmap(ST2_55)                ; preserve ST2 register (for ARMS and ARxLC)

;//-----------------------------------------------------------------------------
;// Initialize
;//-----------------------------------------------------------------------------

        BSET    AR1LC                         ; circular addressing for AR1
        .arms_off
        BCLR    ARMS                          ; reset ARMS bit
        
        MOV     T0, T1                        ; n in T1
        SFTL    T1, #1                        ; 2*n
        MOV     T1, mmap(BK03)                ; circular buffer size is 2*n 
        MOV     T1, mmap(BK47)                ; circular buffer size is 2*n 
        MOV     AR1, mmap(BSA01)              ; circular buffer offset 
;
;        Determine if in-place or off-place computation
;
        MOV     XAR1, AC0
        MOV     XAR0, AC1                     
        
        SUB     AC1, AC0                      ; compare input and output pointers 
        MOV     T0, HI(AC2)                   ; used for log2(N) computation
        
        MOV     #0, AR1                       ; output pointer (circular)
     || BCC     off_place, AC0 != #0          ; if x<>y, do off-place bit reversal

;//-----------------------------------------------------------------------------
;// In-place bit reversal
;//-----------------------------------------------------------------------------
in_place: 
        BSET    AR0LC                        ; circular addressing for AR0
        BSET    AR2LC                        ; circular addressing for AR2
        BSET    AR3LC                        ; circular addressing for AR3
        BSET    AR4LC                        ; circular addressing for AR4
        
        MOV     AR0, mmap(BSA23)             ; circular buffer offset 
        MOV     AR0, mmap(BSA45)             ; circular buffer offset
        MOV     AR0, mmap(BSA01)             ; circular buffer offset 

        MOV     #0, AR0                      ; output pointer (circular)
;
;    Initialize extended part of XARn
;
        MOV     XAR0,XAR2
        MOV     XAR0,XAR3
        MOV     XAR0,XAR4
;
;    Compute log2(N)
;
        EXP     AC2, T1                                ; Hi(AC2) = N
     || MOV     #14, AC2
        SUB     T1,AC2                                ; AC2 = log2(N)
;
;        Test if log2(N) is odd
;
        AND     #1, AC2, AC3
        BCC     log2n_odd, AC3 != #0        ; branch to log2(N) odd processing
;
;         process for log2(N) even
;        
;
;   Compute 2^[log2(N)/2]-1           
;                               
        SFTL    AC2, #-1                        ; log2(N)/2
        MOV     AC2, T1
        NEG     T1
     || MOV     T0, AC2
        SFTL    AC2, T1              ; 2^[log2(N)/2]                                
           
        SUB     #2, AC2, AC3
        MOV     AC3, mmap(BRC0)   ; 2^[log2(N)/2]-2 
            
        MOV     AC2, T0
     || SFTL    AC2, #1 
        MOV     AC2, T1             
        MOV     #1-1, AR1          ; inner loop counter

     || RPTBLOCAL evenOuterLoopEnd-1
               MOV     AR1, BRC1
               AMAR    *(AR0+T0B)
               AMAR    *(AR2+T1)
               AMOV    AR0, AR4
            || ADD     #1, AR1
               AMOV    AR2, AR3
            || RPTBLOCAL evenInnerLoopEnd-1
                        NOP_16                    ; avoid CPU_116 
                     || MOV     dbl(*AR4), AC0
                        MOV     dbl(*AR3), dbl(*(AR4+T1))
                        MOV     AC0, dbl(*(AR3+T0B))
evenInnerLoopEnd:
evenOuterLoopEnd:

        B        done  

log2n_odd:
;
;   Compute 2^[(log2(N)-1)/2]           
;    
        SUB     #1, AC2               ;  log2(N)-1          
        SFTL    AC2, #-1                        ; (log2(N)-1)/2
        MOV     AC2, T1
        ADD     #1, T1
        NEG     T1
    ||  MOV     T0, AC2
        SFTL    AC2, T1                                ;2^[(log2(N)-1)/2]
        
        SUB     #2, AC2, AC3
        MOV     AC3, mmap(BRC0) 
        SFTL    AC2, #1   
        MOV     AC2, T0
        SFTL    AC2, #1 
        MOV     AC2, T1             
            
        MOV     #2-1, AR1         ; inner loop counter
    ||  RPTBLOCAL  oddOuterLoopEnd-1
                MOV     AR1, BRC1
                AMAR    *(AR0+T0B)
                AMAR    *(AR0+T0B)
                AMAR    *(AR2+T1)
                AMOV    AR0, AR4
             || ADD     #2, AR1    ; next time, go thru inner loop 2 extra times
                AMOV    AR2, AR3
             || RPTBLOCAL oddInnerLoopEnd-1
                        NOP_16                    ; avoid CPU_116 
                     || MOV     dbl(*AR4),AC0 
                        MOV     dbl(*AR3),dbl(*(AR4+T0))
                        MOV     AC0,dbl(*(AR3+T0B))
oddInnerLoopEnd:
oddOuterLoopEnd:

        B        done  
         
        
;//-----------------------------------------------------------------------------
;// Off-place bit reversal
;//-----------------------------------------------------------------------------
off_place: 
        SUB     #1, T0, T1              ;
        MOV     T1, BRC0                ; BRC0 = n - 1                                                 
 
        RPTBLOCAL   NotOverlaidLoopEnd-1            
                MOV    dbl(*AR0+), AC0
                MOV    AC0, dbl(*(AR1+T0B))  

NotOverlaidLoopEnd:

;//-----------------------------------------------------------------------------
;// Context Restore
;//-----------------------------------------------------------------------------
done:
        POP     mmap(ST2_55)               ; restore ST2_55 register
        BSET    ARMS                       ; restore C environment 
     || RET                                ; return   

        .end     

 