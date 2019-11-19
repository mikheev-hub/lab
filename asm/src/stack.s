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

loop									; Бесконечный цикл
    MOV32   R7, #0x01
	MOV32   R5, #0x5
	BL      fun1
    B       loop
	
	ENDP

fun1 PROC
	PUSH    {LR, R7}
	MOV     R7, #0xF
	BL      fun2
	MOV     R5, #0xC
	BX      LR
	ENDP
    
fun2 PROC
	POP     {R7}
	POP     {LR}
	MOV     R1, #0xD
	BX      LR
	ENDP
	
		
    END