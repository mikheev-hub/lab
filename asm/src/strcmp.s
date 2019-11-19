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
    MOV32      R0, START_ADDRESS1              ; Start address of array1
	
	MOV        R10, #0x61
	STR        R10, [R0]
	LDR        R11, [R0]
	ADD        R0,  #0x1
	
	MOV        R10, #0x62
	STR        R10, [R0]
	LDR        R11, [R0]
	ADD        R0,  #0x1
	
	MOV        R10, #0x63
	STR        R10, [R0]
	LDR        R11, [R0]
	MOV32      R0, START_ADDRESS1 

	
	
	
	MOV32      R1, START_ADDRESS2              ; Start address of array2

    MOV        R10, #0x61
	STR        R10, [R1]
	LDR        R11, [R1]
	ADD        R1,  #0x1
	
	MOV        R10, #0x62
	STR        R10, [R1]
	LDR        R11, [R1]
	ADD        R1,  #0x1
	
	MOV        R10, #0x63
	STR        R10, [R1]
	LDR        R11, [R1]
	MOV32      R1,  START_ADDRESS2

    MOV        R10, #0x0
	MOV        R11, #0x0
	
	
	
	LDR        R10, [R1]
    CMP        R10, #0x0D
    ITE        NE
	MOVNE      R2,  #0x01
    MOVEQ	   R2, #0x00
	
	LDR        R11, [R3]
	CMP        R11, #0x0D
	ITE        NE
	MOVNE      R4, #0x01
	MOVEQ      R4, #0x00
	

	AND        R5, R2, R4
	TST        R5, #0x01
	ITTTT      EQ
	BLEQ       if_1
	ADDEQ      R1, #0x01
	ADDEQ      R3, #0x01
	BEQ        main
	
	AND        R5, R2, R4
	TEQ        R5, #0x00
	
	ENDP
	END
	

if_1           PROC
	CMP        R10, R11
	ITE        CC
	BLCC       result_bad
    BLCS       result_bad
	BX         LR
	ENDP

ifi    PROC
	CMP        R10, #0x0D
	IT         NE
	BLNE       result_bad
	CMP        R11, #0x0D
	IT         NE
    BLNE       result_bad
	BX         LR
	ENDP

	
 ;result_bad  PROC
    ;MOV        R3, #0x00
    ;ENDP	

	
	
	

