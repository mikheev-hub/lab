    PRESERVE8							; 8-битное выравнивание стека
	THUMB								; Режим Thumb (AUL) инструкций

	GET	config.s						; include-файлы
	GET	stm32f10x.s	

	AREA RESET, CODE, READONLY

	; Таблица векторов прерываний
	DCD STACK_TOP						; Указатель на вершину стека
	DCD Reset_Handler					; Вектор сброса

	ENTRY								; Точка входа в программу

Reset_Handler	PROC					; Вектор сброса
	EXPORT  Reset_Handler				; Делаем Reset_Handler видимым вне этого файла

main									; Основная подпрограмма

	MOV32	R0, PERIPH_BB_BASE + \
			RCC_APB2ENR * 32 + \
			4 * 4						; вычисляем адрес для BitBanding 4-го бита регистра RCC_APB2ENR
										; BitAddress = BitBandBase + (RegAddr * 32) + BitNumber * 4
	MOV		R1, #1						; включаем тактирование порта C (в 4-й бит RCC_APB2ENR пишем '1`)
	STR 	R1, [R0]					; загружаем это значение
	
	MOV32	R0, GPIOC_CRH				; адрес порта
	MOV		R1, #0x222					; 12-битная маска настроек для Input, pull-up / pull-down ("100010001000")
	LDR		R2, [R0]					; считать порт
    BFI		R2, R1, #0, #12    			; скопировать биты маски в позицию PIN0, PIN1, PIN2
    STR		R2, [R0]		


	MOV32	R0, PERIPH_BB_BASE + \
			RCC_APB2ENR * 32 + \
		    2 * 4 
	MOV32	R1, #1 
	STR 	R1, [R0] 

	MOV32 	R0, GPIOA_CRL 
	MOV32	R1, #0x222888 
	LDR 	R2, [R0] 
	BFI 	R2, R1, #0 ,#24 
	STR 	R2, [R0] 

	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN0 << 16) 
	MOV 	R2, #(PIN1 << 16) 
	MOV 	R3, #(PIN2 << 16) 
	MOV 	R4, #(PIN3 << 16) 
	MOV 	R5, #(PIN4 << 16) 
	MOV 	R6, #(PIN5 << 16) 
	ORR 	R1, R2 
	STR 	R1, [R0] 
	ORR 	R3, R4 
	STR 	R3, [R0] 
	ORR 	R5, R6 
	STR 	R5, [R0]
	
	
	;------------------
	; Бесконечный цикл
	;------------------		
loop	
	;--------------------------------
	; Подпрограмма обработки столбцов 
	;--------------------------------		
;**Первый столбец А0**	
	MOV32	R0, GPIOA_IDR
	LDR		R1, [R0]
	TST		R1, #0x1	
	BLEQ    button1
	
;**Второй столбец А1**		
	MOV32	R0, GPIOA_IDR
	LDR		R1, [R0]
	TST		R1, #0x2	
	BLEQ    button2

;**Третий столбец А2**		
	MOV32	R0, GPIOA_IDR
	LDR		R1, [R0]
	TST		R1, #0x4	
	BLEQ    button3
	
	B 		loop	
	ENDP
		
	;--------------------------------
	; Подпрограмма обработки строк 
	;--------------------------------

;**Первая строка А3**	
button1 	PROC
    
	PUSH	{LR}

str1_1
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN3)
	STR		R1, [R0]
	MOV32	R0, GPIOA_IDR
	LDR		R1, [R0]
	TST		R1, #0x1
	BLNE	tab_1
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN3 << 16)
	STR		R1, [R0]


str1_2
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN4)
	STR		R1, [R0]
	MOV32	R0, GPIOA_IDR
	LDR		R1, [R0]
	TST		R1, #0x1
	BLNE	tab_2
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN4 << 16)
	STR		R1, [R0]

str1_3
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN5)
	STR		R1, [R0]
	MOV32	R0, GPIOA_IDR
	LDR		R1, [R0]
	TST		R1, #0x1
	BLNE	tab_3
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN5 << 16)
	STR		R1, [R0]

	POP		{LR}
	BX		LR	
	
	ENDP

;**Вторая строка А4**

button2 	PROC
    
	PUSH	{LR}

str2_1
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN3)
	STR		R1, [R0]
	MOV32	R0, GPIOA_IDR
	LDR		R1, [R0]
	TST		R1, #0x2
	BLNE	tab_4
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN3 << 16)
	STR		R1, [R0]


str2_2
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN4)
	STR		R1, [R0]
	MOV32	R0, GPIOA_IDR
	LDR		R1, [R0]
	TST		R1, #0x2
	BLNE	tab_5
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN4 << 16)
	STR		R1, [R0]

str2_3
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN5)
	STR		R1, [R0]
	MOV32	R0, GPIOA_IDR
	LDR		R1, [R0]
	TST		R1, #0x2
	BLNE	tab_6
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN5 << 16)
	STR		R1, [R0]

	POP		{LR}
	BX		LR	
	
	ENDP


;**Третья строка А5**	

button3 	PROC
    
	PUSH	{LR}

str3_1
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN3)
	STR		R1, [R0]
	MOV32	R0, GPIOA_IDR
	LDR		R1, [R0]
	TST		R1, #0x4
	BLNE	tab_7
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN3 << 16)
	STR		R1, [R0]


str3_2
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN4)
	STR		R1, [R0]
	MOV32	R0, GPIOA_IDR
	LDR		R1, [R0]
	TST		R1, #0x4
	BLNE	tab_8
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN4 << 16)
	STR		R1, [R0]

str3_3
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN5)
	STR		R1, [R0]
	MOV32	R0, GPIOA_IDR
	LDR		R1, [R0]
	TST		R1, #0x4
	BLNE	tab_9
	MOV32 	R0, GPIOA_BSRR 
	MOV 	R1, #(PIN5 << 16)
	STR		R1, [R0]

	POP		{LR}
	BX		LR	
	
	ENDP

;*******Кнопки**********

tab_1		PROC
		
 PUSH	{LR,R7}
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10)
	 STR	R1, [R0] 
	 MOV	R7, #1
	
     BL delay

	 
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10 << 16)
	 STR	R1, [R0] 
	 
	 BL	delay

	 POP	{LR, R7}
	 BX		LR	
	 ENDP
tab_2		PROC
	
	
 PUSH	{LR,R7}
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10)
	 STR	R1, [R0] 
	 MOV	R7, #2
	
     BL delay

	 
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10 << 16)
	 STR	R1, [R0] 
	 
	 BL	delay

	 POP	{LR, R7}
	 BX		LR	
	 ENDP

tab_3		PROC
		
 PUSH	{LR,R7}
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10)
	 STR	R1, [R0] 
	 MOV	R7, #3
	
     BL delay

	 
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10 << 16)
	 STR	R1, [R0] 
	 
	 BL	delay

	 POP	{LR, R7}
	 BX		LR	
	 ENDP
tab_4		PROC
		
 PUSH	{LR,R7}
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10)
	 STR	R1, [R0] 
	 MOV	R7, #4
	
     BL delay

	 
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10 << 16)
	 STR	R1, [R0] 
	 
	 BL	delay

	 POP	{LR, R7}
	 BX		LR	
	 ENDP
tab_5		PROC
		
 PUSH	{LR,R7}
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10)
	 STR	R1, [R0] 
	 MOV	R7, #5
	
     BL delay

	 
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10 << 16)
	 STR	R1, [R0] 
	 
	 BL	delay

	 POP	{LR, R7}
	 BX		LR	
	 ENDP
tab_6		PROC
		
 PUSH	{LR,R7}
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10)
	 STR	R1, [R0] 
	 MOV	R7, #6
	
     BL delay

	 
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10 << 16)
	 STR	R1, [R0] 
	 
	 BL	delay

	 POP	{LR, R7}
	 BX		LR	
	 ENDP
		 
tab_7		PROC
		
 PUSH	{LR,R7}
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10)
	 STR	R1, [R0] 
	 MOV	R7, #7
	
     BL delay

	 
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10 << 16)
	 STR	R1, [R0] 
	 
	 BL	delay

	 POP	{LR, R7}
	 BX		LR	
	 ENDP
		 

tab_8		PROC
 PUSH	{LR,R7}
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10)
	 STR	R1, [R0] 
	 MOV	R7, #8
	
     BL delay

	 
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10 << 16)
	 STR	R1, [R0] 
	 
	 BL	delay

	 POP	{LR, R7}
	 BX		LR	
	 ENDP
		 
tab_9		PROC
		
 PUSH	{LR,R7}
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10)
	 STR	R1, [R0] 
	 MOV	R7, #9
	
     BL delay

	 
	 MOV32 	R0, GPIOC_BSRR
	 MOV 	R1, #(PIN10 << 16)
	 STR	R1, [R0] 
	 
	 BL	delay

	 POP	{LR, R7}
	 BX		LR	
	 ENDP
		 
delay		PROC						; Подпрограмма задержки
	PUSH	{R0,R1}						; Загружаем в стек R0, т.к. его значение будем менять
	LDR		R0,=DELAY_VAL				; псевдоинструкция Thumb (загрузить константу в регистр)
	MUL		R0,R7
delay_loop
	SUBS	R0, #1						; SUB с установкой флагов результата
	IT 		NE
	BNE		delay_loop					; переход, если Z==0 (результат вычитания не равен нулю)
	POP		{R0,R1}						; Выгружаем из стека R0
	BX		LR							; выход из подпрограммы (переход к адресу в регистре LR - вершина стека)
	ENDP

	END