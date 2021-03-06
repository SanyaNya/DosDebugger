#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <conio.h>
#include <dos.h>
#include "misc\ints.h"
#include "context\context.h"
#include "dbgr\dbgr.h"
#include "prgldr\prgldr.h"

uint16_t prgIP;
uint16_t prgCS;

void PrintFullContext(CPU_Context* pcontext, uint16_t programCS, uint16_t programIP)
{
	printf("| ax | cx | dx | bx | sp | bp | si | di | cs | ds | es | ss | ip | flags |\n");
	printf("|%4X|%4X|%4X|%4X|%4X|%4X|%4X|%4X|%4X|%4X|%4X|%4X|%4X|%7X|\n", pcontext->AX, pcontext->CX, pcontext->DX, pcontext->BX, pcontext->SP, pcontext->BP, pcontext->SI, pcontext->DI, programCS, pcontext->DS, pcontext->ES, pcontext->SS, programIP, pcontext->FLAGS);
}

void TraceHandler(CPU_Context* pcontext, uint16_t programCS, uint16_t programIP, uint16_t programNextCS, uint16_t programNextIP)
{
	printf("\nTraceHandler %X:%X -> %X stack: %X %X %X\n", programCS, programIP, programNextIP, *FARPTR(pcontext->SS, pcontext->SP), *FARPTR(pcontext->SS, pcontext->SP+2), *FARPTR(pcontext->SS, pcontext->SP+4));
	PrintFullContext(pcontext, programCS, programIP);
	_getch();
	
	if((programNextIP == 0) && (programNextCS == prgCS))
		stopDebugProgram();
}

int main()
{
	prgCS = LoadProgram("code.com", &ProgramContext, &prgIP);
	if(!prgCS)
	{
		printf("Cannot load code.com err:%i", errno);
		return 0;
	}
	
	startDebugProgram(prgCS, prgIP, TraceHandler);
	
	return 0;
}
