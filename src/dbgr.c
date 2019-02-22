#include <stdint.h>
#include "hdrs\ints.h"
#include "hdrs\context.h"
#include "hdrs\dbgr.h"

//Принцип работы:
//Флаг TF устанавливается в 1, тем самым после выполнения каждой инструкции вызывается прерывание 1.
//Обработчик прерывания 1 заменяется на собственный

#define TRACE_FLAGS 0x300
#define TRACE_INTERRUPT 1

#define PRG_PUSH(val)                               \
ProgramContext.SP -= 2;                             \
*FARPTR(ProgramContext.SS, ProgramContext.SP) = val

void IRET();
#pragma aux IRET = \
"iret";

static void POP_SI();
#pragma aux POP_SI = \
"pop si";

static void POP_BP();
#pragma aux POP_BP = \
"pop bp";

CPU_Context ProgramContext;
static CPU_Context DebuggerContext;

static uint16_t programCS, programIP;
static uint16_t oldIntHandlerCS, oldIntHandlerIP;
static void(*traceHandler)(CPU_Context*, uint16_t, uint16_t, uint16_t, uint16_t);

static void __cdecl intHandler(/*uint16_t ip, uint16_t cs, uint16_t flags*/);

void __cdecl startDebugProgram(uint16_t cs, uint16_t ip, void(*handler)(CPU_Context*, uint16_t, uint16_t, uint16_t, uint16_t))
{
	//Инструкции в начале функции:
	//push si
	//push bp
	//mov bp, sp
	
	SAVE_CONTEXT(&DebuggerContext);
	
	//Ставим свой обработчик прерывания 1, сохраняя старый
	GET_INT(TRACE_INTERRUPT, oldIntHandlerCS, oldIntHandlerIP);
	SET_INT(TRACE_INTERRUPT, GET_CS(), intHandler);
	
	programCS = cs;
	programIP = ip;
	traceHandler = handler;
	
	//Заталкиваем в стек отлаживаемой программы аргументы для iret
	PRG_PUSH(TRACE_FLAGS);
	PRG_PUSH(programCS);
	PRG_PUSH(programIP);
	
	//Загружаем контекст программы и начинаем ее выполнение по адресу cs:ip и TF=1
	LOAD_CONTEXT(&ProgramContext);
	IRET();
}

void __stdcall stopDebugProgram()
{
	//Восстанавливаем прошлый обработик прерывания 1
	SET_INT(TRACE_INTERRUPT, oldIntHandlerCS, oldIntHandlerIP);
	
	//Загружаем контекст отладчика, который был сохранен в начале startDebugProgram
	LOAD_CONTEXT(&DebuggerContext);
	//Восстанавливаем регистры из стека, которые были сохранены в начале startDebugProgram
	POP_BP();
	
	//На вершине стека остается адрес возврата из startDebugProgram
	//ret
}

//Обработчик прерывания 1
//Вызывает пользовательскую функцию traceHandler
static void __cdecl intHandler(/*uint16_t ip, uint16_t cs, uint16_t flags*/)
{
	//Инструкции в начале функции:
	//push si
	
	//Убираем изменения которые сделаны инструкциями в начале функции
	POP_SI();
	
	//Сохраняем контекст отлаживаемой программы
	//и загружаем контекст отладчика для вызова traceHandler
	SAVE_CONTEXT(&ProgramContext);
	LOAD_CONTEXT(&DebuggerContext);
	
	ProgramContext.SP += 6; //Возвращаем SP к значению до вызова прерывания
	
	traceHandler(&ProgramContext, programCS, programIP, *FARPTR(ProgramContext.SS, ProgramContext.SP-4), *FARPTR(ProgramContext.SS, ProgramContext.SP-6));

	ProgramContext.SP -= 6; //Возвращаем SP к значению после вызова прерывания
	
	//Обновляем позицию программы
	programCS = *FARPTR(ProgramContext.SS, ProgramContext.SP+2);
	programIP = *FARPTR(ProgramContext.SS, ProgramContext.SP);
	
	//Загружаем контекст программы и возвращаемся обратно в нее
	LOAD_CONTEXT(&ProgramContext);
	IRET();
}
