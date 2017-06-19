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
; Processor:   C55xx
; Description: Radix-2 DIT complex FFT using normal input data
;    and bit-reversed twiddle table (length N/2, cosine/sine format)
;    All stages are in radix-2.
;
; Usage:    void cfft_SCALE (DATA *x, ushort nx);
;
; Limitations:
;   nx must be a power of 2 between 8 and 1024
;
; Benchmarks:
;   Cycles:
;     nx =   8:    162
;     nx =  16:    307
;     nx =  32:    571
;     nx =  64:   1155
;     nx = 128:   2459
;     nx = 256:   5363
;     nx = 512:  11787
;     nx =1024:  25891
;   Code Size (in bytes):  
;     .text            467
;     .const:twiddle  2048 (shared by cfft, cifft, unpack, unpacki)
;
; History:
; 	- 07/18/2002	Sira fixed the bug in the kernal.
;	- 07/02/2003	D. Elam added support for 8 point
;	- 07/17/2003	C. Iovescu changed the way the twiddle table is included
;       - 06/17/2012    Craig Leeds: optimized housekeeping ; removed .bss
;****************************************************************

;-----------------------------------------------------------------------
; Arguments passed to _fft
;
; AR0       ->    fftdata pointer
; T0        ->    fft size
;
;-------------------------------------------------------------------------



;//-----------------------------------------------------------------------------
;// Array declarations
;//-----------------------------------------------------------------------------

	.ref	twiddle  ; include twiddle table	
	         
        .def _cfft_SCALE     ; make function visible to other fnct
        .cpl_on
        .arms_off     ; enable assembler for arms mode
        .mmregs
        .noremark    5579, 5573, 5549, 5684
  
; Stack frame
; -----------
RET_ADDR_SZ       .set 1            ;return address
REG_SAVE_SZ       .set 5            ;save-on-entry registers saved
FRAME_SZ          .set 2            ;local variables
ARG_BLK_SZ        .set 0            ;argument block

PARAM_OFFSET      .set ARG_BLK_SZ + FRAME_SZ + REG_SAVE_SZ + RET_ADDR_SZ


; Local variables
; --------------      
       
       .asg    0, data_ptr
       .asg    1, data_sz

;//-----------------------------------------------------------------------------
;// Register aliases
;//-----------------------------------------------------------------------------
		.asg	T3,groups		; number of butterfly groups
		.asg	T1,index		; addess adjustment
		.asg	AR7,bflies		; number of butterflies/group
		.asg	AR4,k			; outer loop counter
		.asg	AR2,a			; top butterfly pointer
		.asg	AR3,b			; bottom butterfly pointer
		.asg	AR5,temp		; temp register	

        .text           

_cfft_SCALE:

;
; Save any save-on-entry registers that are used
; Modification of status registers
;----------------------------------------------------------------
;
        PSH     T3, T2
     || BCLR    #ARMS, ST2_55          
        PSHBOTH XAR5
     || BSET    FRCT
        PSHBOTH XAR6
     || BSET    SATD
        PSHBOTH XAR7
;
; Allocate the local frame and argument block
;----------------------------------------------------------------
        AADD    #-(ARG_BLK_SZ + FRAME_SZ), SP
; save all save-on-entry regs that are modified

;-----------------------------------------------------------------------

; Define CSR for scaling loop

        SUB      #2, T0, T1                          
        MOV      T1, CSR                               ; CSR = fftsize - 2
 
;
; Save entry values for later
;----------------------------------------------------------------

        MOV     AR0, *SP(data_ptr) ; 
        MOV     T0, *SP(data_sz)   ;
        
;*******************************************************************************
; SCALING LOOP: Data scaled by 2 before first stage 
;*******************************************************************************
        
        AMAR    *AR0, XAR1                             ; AR0 = #fftdata
        MOV     dbl(*AR0+), AC0                        ; scale by 2 - prime the pipe
        RPT     CSR
                MOV      AC0 >> #1, dual(*AR1+)
              ||MOV      dbl(*AR0+), AC0
        MOV     AC0 >> #1, dual(*AR1+)

;*******************************************************************************
; END SCALING LOOP
;*******************************************************************************

; radix-2 stage 1
; 
        MOV     #2, T1
        MOV     XAR0, XAR2
        MOV     XAR0, XAR4
        MOV     *SP(data_ptr), AR2    ; AR2 = #fftdata  
        MOV     AR2, AR4               ; AR4 = #fftdata  
        MOV     AR2, AR0
        ADD     T0, AR0               ; AR0 = fftdata + fft_size
        MOV     XAR0, XAR5            ; AR5 = fftdata + fft_size
        MOV     T0, T2
        SFTS    T2, #-1               ; T2 = fft_size/2 
        MOV     T2, AR7
        SUB     #2, AR7
        MOV     AR7, BRC0
        
                 ; AC2=Qr[0]:Qi[0] 
        MOV     dbl(*(AR0+T1)), AC2      
                
                ; ac0 = Pr[0]+Qr[0]:Pi[0]+Qi[0]
                ;     = Pr'[0]:Pr'[0]
           
        ADD     dual(*AR2), AC2, AC0 
   
                ; AC1 = Pr[0]-Qr[0]:Pi[0]-Qi[0]
                ;     = Qr'[0]:Qi'[0]
                ; store P'[0]
        SUB     AC2, dual(*(AR2+T1)), AC1    
     || MOV     AC0>>#1, dual(*(AR4+T1))               
                                       
                 
        RPTBLOCAL r2_loop00-1          
                MOV     dbl(*(AR0+T1)), AC2         ; load Q            
             || MOV     AC1>>#1, dual(*(AR5+T1))    ; store new Q   
                ADD     dual(*AR2), AC2, AC0        ; new P   
                SUB     AC2, dual(*(AR2+T1)), AC1   ; new Q 
             || MOV     AC0>>#1, dual(*(AR4+T1))    ; store new P  
r2_loop00:      
             
        MOV     AC1>>#1, dual(*(AR5+T1))            ; store new Q   

; radix-2 stage 2
;
; groupe #1 twiddle = #1  

        MOV     *SP(data_ptr), AR2     ; AR2 = #fftdata  
        MOV     AR2, AR4               ; AR4 = #fftdata  
        MOV     AR2, AR0
        ADD     T2, AR0                ; AR0 = fftdata + fft_size/2
        MOV     AR0, AR5               ; AR5 = fftdata + fft_size/2  
        MOV     T2, T3                 ; T3= fft_size/2
        SFTS    T2, #-1                ; T2 = fft_size/4 
        SUB     #2, T2
        MOV     T2, BRC0               ; BRC0 = fft_size/4 - 2
 
                   ; AC2=Qr[0]:Qi[0] 
        MOV     dbl(*(AR0+T1)), AC2       
                
                ; ac0 = Pr[0]+Qr[0]:Pi[0]+Qi[0]
                ;     = Pr'[0]:Pr'[0]
                ; CDP = #twiddle
        ADD     dual(*AR2), AC2, AC0 
        
                ; AC1 = Pr[0]-Qr[0]:Pi[0]-Qi[0]
                ;     = Qr'[0]:Qi'[0]
                ; store P'[0]
        SUB     AC2, dual(*(AR2+T1)), AC1    
     || MOV     AC0>>#1, dual(*(AR4+T1))               
                 
        RPTBLOCAL r2_loop01-1          
                MOV     dbl(*(AR0+T1)), AC2         ; load Q            
             || MOV     AC1>>#1, dual(*(AR5+T1))    ; store new Q   
                ADD     dual(*AR2), AC2, AC0        ; new P   
                SUB     AC2, dual(*(AR2+T1)), AC1   ; new Q 
             || MOV     AC0>>#1, dual(*(AR4+T1))    ; store new P  
r2_loop01:      

        MOV    AC1>>#1, dual(*(AR5+T1))     ; store new Q    
 
; radix-2 stage 2
;
; groupe #2 twiddle = #-1    

        MOV     *SP(data_ptr), AR2     ; AR2 = #fftdata  
        ADD     T0, AR2                ; AR2 = #fftdata + fft_size
        MOV     AR2, AR4               ; AR4 = #fftdata + fft_size  
        MOV     AR2, AR0
        ADD     T3,  AR0               ; AR0 = #fftdata + 3/2*fft_size 
        MOV     AR0, AR1
        ADD     #1, AR1
        MOV     AR0, AR5               ; AR5 = #fftdata + 3/2*fft_size
        MOV     T2, BRC0               ; BRC0 = fft_size/4 - 2                                 
        AMOV    #twiddle, XAR3         
        ADD     #2, AR3                
        MOV     XAR3, XCDP            
                      
                    ; AC2=Qr[n]*Wr                                    
                    ; AC3=Qi[n]*Wr
        MPY     *AR0, *CDP+, AC2               
      ::MPY     *AR1, *CDP+, AC3  
       
                   ; AC3=Qi[n]*Wr+Qr[n]*Wi
                    ; AC2=Qr[n]*Wr-Qi[n]*Wi

        MASR    *(AR0+T1), *CDP-, AC3
      ::MACR    *(AR1+T1), *CDP-, AC2 
        
                    ; AC2=(Qr[n]*Wr-Qi[n]*Wi):(Qi[n]*Wr+Qr[n]*Wi)

        OR      AC3 << #-16, AC2
         
                    ; hi(AC0)=Pr[n]+(Qr[n]*Wr-Qi[n]*Wi)=Pr'[n]
                    ; lo(AC0)=Pi[n]+(Qr[n]*Wi+Qi[n]*Wr)=Pi'[n]
                             
        ADD     dual(*AR2), AC2, AC0            
   
                       ; hi(AC1)=Pr[n]-(Qr[n]*Wr-Qi[n]*Wi)=Qr'[n] 
                    ; lo(AC1)=Pi[n]-(Qr[n]*Wi+Qi[n]*Wr)=Qi'[n]
                    ; store Pr'[n]:Pi'[n]

        SUB     AC2, dual(*(AR2+T1)), AC1
     || MOV     AC0>>#1, dual(*(AR4+T1)) 
        
        RPTBLOCAL r2_loop02-1

                          ; AC2=Qr[n]*Wr                                    
                          ; AC3=Qi[n]*Wr
                MPY     *AR0, *CDP+, AC2               
              ::MPY     *AR1, *CDP+, AC3                           
        
                          ; AC3=Qi[n]*Wr+Qr[n]*Wi
                          ; AC2=Qr[n]*Wr-Qi[n]*Wi  --correct commenting!
          
                MASR    *(AR0+T1), *CDP-, AC3   
             :: MACR    *(AR1+T1), *CDP-, AC2  
            
                          ; AC2=(Qr[n]*Wr-Qi[n]*Wi):(Qi[n]*Wr+Qr[n]*Wi)
         
                OR      AC3 << #-16, AC2      
                          ; hi(AC0)=Pr[n]+(Qr[n]*Wr-Qi[n]*Wi)=Pr'[n]
                          ; lo(AC0)=Pi[n]+(Qr[n]*Wi+Qi[n]*Wr)=Pi'[n]
                          ; store Qr'[n-1]:Qi'[n-1]

                ADD     dual(*AR2), AC2, AC0
             || MOV     AC1>>#1, dual(*(AR5+T1)) 
    
                          ; hi(AC1)=Pr[n]-(Qr[n]*Wr-Qi[n]*Wi)=Qr'[n] 
                          ; lo(AC1)=Pi[n]-(Qr[n]*Wi+Qi[n]*Wr)=Qi'[n]
                          ; store Pr'[n]:Pi'[n]

                SUB     AC2, dual(*(AR2+T1)), AC1
             || MOV     AC0>>#1, dual(*(AR4+T1)) 
r2_loop02:                                 ; end of butterfly loop
                              
        MOV     *SP(data_sz), T0              
        SUB     #8, T0              
        MOV     AC1>>#1, dual(*(AR5+T1))  
        BCC     final_stage, T0==#0        ; branch to final stage for 8-point fft

;-----------------------------------------------------------------------
; radix-2 stages (stages 3->log2(FFT_SIZE) )
        
        ; main initialization
        ; --------------------
        
        MOV     *SP(data_ptr), AR0  ; AR0 = #fftdata
            ; 1 - circular buffer initializations

        
            ; modify ST2 to select linear or circular addressing modes
        OR      #0x57 , mmap(ST2_55)  ; circular AR0, AR1, AR2, AR4 and AR6
         
            ; circular buffer starting addresses
        ADD     #2, AR0,AR1
        MOV     AR0, mmap(BSA23)    ; P  leg
        MOV     AR0, mmap(BSA45)    ; P' leg    (in-place butterflies)
        MOV     AR1, mmap(BSA01)    ; Q  leg
        MOV     AR1, mmap(BSA67)    ; Q' leg    (in-place butterflies)
                            
            ; circular buffer sizes 
        MOV     *SP(data_sz), T0
        MOV     T0, T1              ; T1 =  #FFT_SIZE
        SFTS    T0, #1        
        SUB     #2, T0
    
        MOV     T0, mmap(BK03)      ; BK03 = (2*FFT_SIZE-2), 
        MOV     T0, mmap(BK47)      ; BK47 = (2*FFT_SIZE-2), 

            ; 2 - register initializations
        
        MOV     *SP(data_sz), T0
        SFTS    T0, #-1
        SFTS    T0, #-1
        
        MOV     T0, AR5             ; 2*number of groups for stage 3    
        MOV     #2, AR7             ; 1/2 * number of butterflies per group for stage 3
       
        ;AMOV #twiddle, XAR3        ; not modified during iteration of the stages
        MOV     XAR0,XAR1           ; data page = data page of indata
        MOV     XAR0,XAR2           ; data page = data page of indata
        MOV     XAR0,XAR4           ; data page = data page of indata (in-place butterflies)
        MOV     XAR0,XAR6           ; data page = data page of indata (in-place butterflies)
        MOV     XAR3,XCDP           ; data page = data page of twiddles
        
stage:    ; begining of a new stage

        ; stage initialization
        ; --------------------

            ; 1 - loop counter updates
        
            ; butterfly counter update
        SFTS    AR7, #1
     || MOV     #0, AR4 
        SUB     #2, AR7, AR0
        MOV     AR0, BRC1        ; butterflies executed out of the loop

            ; group counter update
        SFTS    AR5, #-1
        SUB     #1, AR5, AR0     
        MOV     AR0, BRC0         

            ; stage "counter" update (T0=0 <=> last stage)
        MOV     AR0, T0

            ; 2 - pointer modifier update
                            
            ; T1 is the pointer modifier                    
        SFTS    T1, #-1        

            ; 3 - index updates                  
            ; AR0 -> Qr[0]
            ; AR1 -> Qi[0]
            ; AR2 -> Pr[0]
            ; AR4 -> Pr'[0]  (performed above to use parallelism)
            ; AR6 -> Qr'[0]
 
        SFTS    AR0, #1
        ADD     #1, AR0, AR1
        MOV     #0, AR2
    ||  MOV     AR0, AR6 
        
; begining of a new group

        ; note:
        ; - circular buffers are used for data pointers so that no
        ;   initialization of data pointers is required before a new
        ;   iteration of a group
        ; - twiddle pointer (CDP) is updated inside the loop
                
        RPTBLOCAL r2_loop1-1
        
                ; AC2=Qr[0]:Qi[0] 
                MOV     dbl(*(AR0+T1)), AC2
            ||  ADD     T1, AR1       

                ; CDP = #twiddle
                MOV     AR3, CDP
                
                ; AC0 = Pr[0]+Qr[0]:Pi[0]+Qi[0]
                ;     = Pr'[0]:Pr'[0]
                ADD     dual(*AR2), AC2, AC0 
        
                ; AC1 = Pr[0]-Qr[0]:Pi[0]-Qi[0]
                ;     = Qr'[0]:Qi'[0]
                ; store P'[0]
                SUB     AC2, dual(*(AR2+T1)), AC1    
             || MOV     AC0 >> #1, dual(*(AR4+T1))               
                                                          
        ; --------------------------------------------------------------        
        ; DIT radix-2 butterfly
        ;
        ; Register usage:
        ; 
        ;     address registers
        ; AR0 = Qr
        ; AR1 = Qi
        ; AR2 = Pr:Pi
        ; AR4 = Pr':Pi'
        ; AR6 = Qr':Qi'
        ; CDP = Wr:Wi
        ;
        ;     temporary registers
        ; T1 pointer modifier (jump between two consecutive legs)
        ; T2, T3 - both modified
        ;
        ;     accu registers
        ; AC0, AC1, AC2, AC3 are all modified
        ; 
        ;
        ; Comments:
        ;
        ; This butterfly supports both in-place and out-if-place
        ; implementations.
        ;
        ; input data assumed to be in order (bit-reversion after fft)
        ;
        ; twiddle table
        ;   - assumed to be in bit-reversed order
        ;     - the twiddle factors stored in the tables are the pairs
        ;      {cos(2*pi*k/N) , sin(2*pi*k/N)} = {Wr[k] , -Wi[k]}
        ;   - first twiddle pair {1,0} is suppressed (since the first
        ;       butterfly is computed out of the inner loop)
        ;   - second twiddle pair {0,1} replaced by {0,-1} (-1 can be
        ;     represented with full precision but not 1)
        ;   - when inner loop starts, CDP should point to the
        ;     third twiddle pair {1/sqrt(2),1/sqrt(2)}
        ;
        ; pointer registers
        ;   AR0 -> Qr[]
        ;   AR1 -> Qi[]
        ;   AR2 -> Pr[]
        ;   AR4 -> Pr'[]
        ;   AR6 -> Qr'[]
        ;   CDP -> W[] (bit-reversed twiddle table)
        ;
        ; --------------------------------------------------------------        
        
                RPTBLOCAL r2_loop2-1

                                    ; AC2=Qr[n]*Wr                                    
                                    ; AC3=Qi[n]*Wr
                        MPY     *AR0, *CDP+, AC2               
                      ::MPY     *AR1, *CDP+, AC3                           
                
                                  ; AC3=Qi[n]*Wr+Qr[n]*Wi
                                  ; AC2=Qr[n]*Wr-Qi[n]*Wi

                        MASR    *(AR0+T1), *CDP+, AC3   
                      ::MACR    *(AR1+T1), *CDP+, AC2   
            
                                  ; AC2=(Qr[n]*Wr-Qi[n]*Wi):(Qi[n]*Wr+Qr[n]*Wi)

                        OR      AC3 << #-16, AC2      
                                  ; hi(AC0)=Pr[n]+(Qr[n]*Wr-Qi[n]*Wi)=Pr'[n]
                                  ; lo(AC0)=Pi[n]+(Qr[n]*Wi+Qi[n]*Wr)=Pi'[n]
                                  ; store Qr'[n-1]:Qi'[n-1]

                        ADD     dual(*AR2), AC2, AC0
                     || MOV     AC1>>#1, dual(*(AR6+T1)) 
    
                                  ; hi(AC1)=Pr[n]-(Qr[n]*Wr-Qi[n]*Wi)=Qr'[n] 
                                  ; lo(AC1)=Pi[n]-(Qr[n]*Wi+Qi[n]*Wr)=Qi'[n]
                                  ; store Pr'[n]:Pi'[n]

                        SUB     AC2, dual(*(AR2+T1)), AC1
                     || MOV     AC0>>#1, dual(*(AR4+T1)) 
r2_loop2:                                         ; end of butterfly loop
        
                MOV     AC1>>#1, dual(*(AR6+T1))  ; store last Qr':Qi'
r2_loop1:                                         ; end of group loop

        SUB     #1, T0
        BCC     stage, T0 != #0       ; end of stage loop             
            
; last r2 stage
; no scaling performed
;
; modify ST2 to select linear or circular addressing modes
final_stage:       
        AND     #0xF000 , mmap(ST2_55) ; restore linear AR0, AR1, AR2, AR4 and AR6
        MOV     #2, T1  
        MOV     *SP(data_sz), T0       ; T0 = #fftsize
        MOV     *SP(data_ptr), AR2     ; AR2 = #fftdata  
        MOV     AR2, AR4               ; AR4 = #fftdata  
        MOV     AR2, AR0
        AADD    T1, AR0                ; AR0 = fftdata + 2 
        MOV     #7, AR1
        ADD     AR2, AR1               ; 
        MOV     AR0, AR5               ; AR5 = fftdata + 2 
        SFTS    T0, #-1  
        SUB     #2, T0
        MOV     T0, BRC0               ; BRC0 = fftsize/2-2 
        MOV     #4, T1
        MOV     AR3, CDP             
                      
              ; AC2=Qr[0]:Qi[0] 
        MOV     dbl(*(AR0+T1)), AC2
                
                ; AC0 = Pr[0]+Qr[0]:Pi[0]+Qi[0]
                ;     = Pr'[0]:Pr'[0]
                ; CDP = #twiddle
        ADD     dual(*AR2), AC2, AC0 
        
                ; AC1 = Pr[0]-Qr[0]:Pi[0]-Qi[0]
                ;     = Qr'[0]:Qi'[0]
                ; store P'[0]
        SUB     AC2, dual(*(AR2+T1)), AC1    
     || MOV     AC0, dbl(*(AR4+T1))               
                                                          
        RPTBLOCAL r2_loop3-1

                          ; AC2=Qr[n]*Wr                                    
                          ; AC3=Qi[n]*Wr
                MPY     *AR0, *CDP+, AC2               
              ::MPY     *AR1, *CDP+, AC3                           
        
                          ; AC3=Qi[n]*Wr+Qr[n]*Wi
                          ; AC2=Qr[n]*Wr-Qi[n]*Wi

                MASR    *(AR0+T1), *CDP+, AC3  
              ::MACR    *(AR1+T1), *CDP+, AC2   
            
                          ; AC2=(Qr[n]*Wr-Qi[n]*Wi):(Qi[n]*Wr+Qr[n]*Wi)
        
                OR      AC3 << #-16, AC2      
                          ; hi(AC0)=Pr[n]+(Qr[n]*Wr-Qi[n]*Wi)=Pr'[n]
                          ; lo(AC0)=Pi[n]+(Qr[n]*Wi+Qi[n]*Wr)=Pi'[n]
                          ; store Qr'[n-1]:Qi'[n-1]

                ADD     dual(*AR2), AC2, AC0
             || MOV     AC1, dbl(*(AR5+T1)) 
    
                          ; hi(AC1)=Pr[n]-(Qr[n]*Wr-Qi[n]*Wi)=Qr'[n] 
                          ; lo(AC1)=Pi[n]-(Qr[n]*Wi+Qi[n]*Wr)=Qi'[n]
                          ; store Pr'[n]:Pi'[n]

                SUB     AC2, dual(*(AR2+T1)), AC1
             || MOV     AC0, dbl(*(AR4+T1)) 
r2_loop3:                             ; end of butterfly loop
                      
        MOV     AC1, dbl(*(AR5+T1))
              
end_benchmark:

;
; Deallocate the local frame and argument block
;----------------------------------------------------------------
        AADD    #(ARG_BLK_SZ + FRAME_SZ), SP

;Context restore 

        POPBOTH XAR7
     || BSET    #ARMS, ST2_55          
        POPBOTH XAR6
     || BCLR    FRCT
        POPBOTH XAR5
     || BCLR    SATD
        POP     T3, T2
        RET 

      .end         
        






