#ifndef DBGR_H
#define DBGR_H

#include <stdint.h>
#include "hdrs\context.h"

//Применение:
//Загрузка программы в память
//Вызов startDebugProgram с позицией старта программы
//Обработка в пользовательской функции traceHandler
//Для остановки отладки - вызов stopDebugProgram

extern CPU_Context ProgramContext;

//Начинает отладку программы по адресу cs:ip
//Пользовательская функция initContext инициализирует начальное состояние регистров и флагов отлаживаемой программы 
//После выполнения каждой инсрукции вызывается пользовательская функция handler, которой передается состояние регистров и флагов отлаживаемой программы
void __cdecl startDebugProgram(uint16_t cs, uint16_t ip, void(*handler)(CPU_Context*, uint16_t, uint16_t, uint16_t, uint16_t));

//Останавливает отладку программы и возвращается к месту вызова startDebugProgram
void __stdcall stopDebugProgram();

#endif
