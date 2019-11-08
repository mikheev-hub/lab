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


main
    MOV32    R1, #START_ADDRESS
	
	MOV      R10, #0x1
	STR      R10, [R1]
	LDR      R11, [R1]
	ADD      R1, #0x1
	
	MOV      R10, #0x2
	STR      R10, [R1]
	LDR      R11, [R1]
	ADD      R1, #0x1
	
	MOV      R10, #0x3
	STR      R10, [R1]
	LDR      R11, [R1]
	ADD      R1, #0x1
	
	MOV      R10, #0x4
	STR      R10, [R1]
	LDR      R11, [R1]
	ADD      R1,  #0x1
	
	MOV      R10, #0x5
	STR      R10, [R1]
	LDR      R11, [R1]
	
	
	MOV      R2, #0x4                   ; Степень полинома
	MOV      R3, #0x2                   ; Значение х
	MOV      R4, #0x0                   ; sum = 0
	MOV      R5, #0x0                   ; i = 0 для pow
	MOV      R7, #0x0                   ; i = 0 для for
	MOV      R6, #0x1                   ; result = 1
	MOV      R8, #0x0                   ; len
	MOV32    R1, #0x20000200
	
for
    CMP      R7, R2
	ITT      LS
	LDRSBLS  R11, [R1]                  ; int a = k[i]
	SUBLS    R8, R2, R5                 ; len - i

	BL       pow
	CMP      R7, R2
	IT       LS
	MULLS    R10, R6, R11                ; a * pow
    CMP      R4, R2
	ITTTT    LS
	ADDLS    R4, R4, R10                ; sum += x
    ADDLS    R7, #0x1                   ; i++
    ADDLS    R1, #0x1                   ; k[i] + 1
    BLS      for    
	
	ENDP
	

pow         PROC
	CMP     R5, R8                    
    ITTT    LT
    MULLT   R6, R6, R3                 ; rez = rez * x	
	ADDLT   R5, #0x1                   ; i++
	BLT     pow
	MOV     R5, #0x0
	BX      LR
	ENDP
		
		
	END
    	