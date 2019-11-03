    PRESERVE8
    THUMB 
    GET    config.s
    GET    stm32f10x.s

    AREA    RESET, CODE, READONLY

    DCD    STACK_TOP
    DCD    Reset_Handler

    ENTRY

Reset_Handler    PROC
    EXPORT    Reset_Handler

main
    MOV32   R0, PERIPH_BB_BASE + \
            RCC_APB2ENR * 32 + \
            4 * 4						
										
    MOV     R1, #1						
    STR     R1, [R0]					

    MOV32   R0, GPIOC_CRH				
    MOV     R1, #0x00000003					
    LDR     R2, [R0]					
    BFI     R2, R1, #0, #4    			
    STR     R2, [R0]					
	

    MOV32   R0, PERIPH_BB_BASE + \
            RCC_APB2ENR * 32 + \
            2 * 4						
										
    MOV     R1, #1						
    STR     R1, [R0]	
	

    MOV32   R0, GPIOA_CRL				
    MOV     R1, #0x00000008					
    LDR     R2, [R0]					
    BFI     R2, R1, #0, #4    			
    STR     R2, [R0]
	
		;;;Pull down;;;;
	MOV32   R0, GPIOA_ODR				
    MOV     R1, #0x0					
    STR     R1, [R0]	
	
	
    MOV     R3, #0x00000000             ; R3 = 0
    MOV     R5,	#0x00000001             ; flag = true
	MOV 	R1, #0x0
	
	
loop
    MOV32   R0, GPIOA_IDR               ; R0 = GPIOA_IDR	
    LDR     R1, [R0]
    MOV     R2, #0x00000001             ; R2 = 0x00000001
    AND     R1, R2                      ; R1 = R1 & R2			
    CMP     R1, #0x00000001             ; R1 == 0x00000001
    IT      EQ                          ; if(button)
    BLEQ    flag
    CMP     R1, #0x00000001
	IT		NE                          ; if(!button) 
    MOVNE	R5, #0x00000001             ; flag = true
	;MOV		R1, #0x00000000
	
    MOV32   R0, GPIOC_BSRR
    MOV     R1, #(PIN8)
    STR     R1, [R0]
    BL      delay
    MOV     R1, #(PIN8 << 16)
    STR     R1, [R0]
    BL      delay
    B       loop
    ENDP
		
flag        PROC
    CMP     R5, #0x00000001             ; if(flag) 
    IT      EQ
    ADDEQ   R3, #0x00000001             ; R3 = R3 + 1
	MOV     R5, #0x00000000             ; if(!flag)=>flag = false
    CMP     R3, #0x00000006
    IT      EQ
    MOVEQ   R3, #0x00000001	
	BX      LR
    ENDP
	
	

	
delay       PROC						
    MOV     R2, #0x00000500
    CMP     R3,	#0x00000000
    ITE     EQ
    MOVEQ   R4, #0x00000001
    UDIVNE  R4, R2, R3                  ; R4 = R2 / R3
delay_loop
    SUBS    R4, #1						
    IT      NE
    BNE     delay_loop					
    BX      LR							
    ENDP
		
	
    END
	