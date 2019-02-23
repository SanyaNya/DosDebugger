#ifndef PRGLDR_H
#define PRGLDR_H

#include <stdint.h>
#include "..\context\context.h"

//Выделяет сегмент для программы и загружает ее туда
//Инициализирует контекст программы (в зависимости от COM или EXE)
//Возвращает сегмент
//В случае ошибки возвращает 0 и изменяет errno
uint16_t LoadProgram(const char* path, CPU_Context* pcontext, uint16_t* pPrgIP);

//Освобождает выделенный под программу сегмент
void FreeProgram(uint16_t seg);

#endif
