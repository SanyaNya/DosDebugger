#ifndef CONTEXT_H
#define CONTEXT_H

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
} CPU_Context;

extern CPU_Context* _pcontext;

void PUSH_AX();
#pragma aux PUSH_AX = \
"push ax";

void POP_AX();
#pragma aux POP_AX = \
"pop ax";

void SET_PCONTEXT(CPU_Context* pcntxt);
#pragma aux SET_PCONTEXT = \
"mov [cs:_pcontext], ax"  \
parm [ax];

//Сохранение контекста по адресу CS:_pcontext
void SaveContext();
#pragma aux SaveContext = \
"push bx"                 \
"mov bx, [cs:_pcontext]"  \
"mov [cs:bx], ax"         \
"mov [cs:bx+2],  cx"      \
"mov [cs:bx+4],  dx"      \
"pop [cs:bx+6]"           \
"mov [cs:bx+8],  sp"      \
"mov [cs:bx+10], bp"      \
"mov [cs:bx+12], si"      \
"mov [cs:bx+14], di"      \
"mov [cs:bx+16], ds"      \
"mov [cs:bx+18], es"      \
"mov [cs:bx+20], ss"      \
"pushf"                   \
"pop [cs:bx+22]"          \
"mov bx, [cs:bx+6]";

#define SAVE_CONTEXT(pcntxt) \
PUSH_AX();                   \
SET_PCONTEXT(pcntxt);        \
POP_AX();                    \
SaveContext();

//Загрузка контекста по адресу CS:_pcontext
void LoadContext();
#pragma aux LoadContext = \
"mov bx, [cs:_pcontext]"  \
"mov ax, [cs:bx]"         \
"mov cx, [cs:bx+2]"       \
"mov dx, [cs:bx+4]"       \
"mov sp, [cs:bx+8]"       \
"mov bp, [cs:bx+10]"      \
"mov si, [cs:bx+12]"      \
"mov di, [cs:bx+14]"      \
"mov ds, [cs:bx+16]"      \
"mov es, [cs:bx+18]"      \
"mov ss, [cs:bx+20]"      \
"push [cs:bx+22]"         \
"popf"                    \
"mov bx, [cs:bx+6]";

#define LOAD_CONTEXT(pcntxt) \
SET_PCONTEXT(pcntxt);        \
LoadContext();

#endif
