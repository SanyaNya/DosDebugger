#ifndef SCONTEXT_H
#define SCONTEXT_H

#include <stdint.h>

typedef struct
{
	uint16_t AX; //00
	uint16_t CX; //02
	uint16_t DX; //04
	uint16_t BX; //06
	uint16_t SP; //08
	uint16_t BP; //10
	uint16_t SI; //12
	uint16_t DI; //14
	uint16_t DS; //16
	uint16_t ES; //18
	uint16_t SS; //20
	uint16_t FLAGS; //22
} Stable_CPU_Context;

extern Stable_CPU_Context* _stable_pcontext;

void STABLE_PUSH_AX();
#pragma aux STABLE_PUSH_AX = \
"push ax";

void STABLE_POP_AX();
#pragma aux STABLE_POP_AX = \
"pop ax";

void STABLE_SET_PCONTEXT(Stable_CPU_Context* pcntxt);
#pragma aux STABLE_SET_PCONTEXT = \
"mov [cs:_stable_pcontext], ax"   \
parm [ax];

//Сохранение контекста по адресу CS:_pcontext
void Stable_SaveContext();
#pragma aux Stable_SaveContext = \
"push bx"                        \
"mov bx, [cs:_stable_pcontext]"  \
"mov [cs:bx], ax"                \
"mov [cs:bx+2],  cx"             \
"mov [cs:bx+4],  dx"             \
"pop [cs:bx+6]"                  \
"mov [cs:bx+8],  sp"             \
"mov [cs:bx+10], bp"             \
"mov [cs:bx+12], si"             \
"mov [cs:bx+14], di"             \
"mov [cs:bx+16], ds"             \
"mov [cs:bx+18], es"             \
"mov [cs:bx+20], ss"             \
"pushf"                          \
"pop [cs:bx+22]"                 \
"mov bx, [cs:bx+6]";

#define STABLE_SAVE_CONTEXT(pcntxt) \
STABLE_PUSH_AX();                   \
STABLE_SET_PCONTEXT(pcntxt);        \
STABLE_POP_AX();                    \
Stable_SaveContext();

//Загрузка контекста по адресу CS:_pcontext
void Stable_LoadContext();
#pragma aux Stable_LoadContext = \
"mov bx, [cs:_stable_pcontext]"  \
"mov ax, [cs:bx]"                \
"mov cx, [cs:bx+2]"              \
"mov dx, [cs:bx+4]"              \
"mov sp, [cs:bx+8]"              \
"mov bp, [cs:bx+10]"             \
"mov si, [cs:bx+12]"             \
"mov di, [cs:bx+14]"             \
"mov ds, [cs:bx+16]"             \
"mov es, [cs:bx+18]"             \
"mov ss, [cs:bx+20]"             \
"push [cs:bx+22]"                \
"popf"                           \
"mov bx, [cs:bx+6]";

#define STABLE_LOAD_CONTEXT(pcntxt) \
STABLE_SET_PCONTEXT(pcntxt);        \
Stable_LoadContext();

#endif
