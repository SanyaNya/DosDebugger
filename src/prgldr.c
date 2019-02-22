#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "hdrs\ints.h"
#include "hdrs\context.h"
#include "hdrs\prgldr.h"

static int loadCode(const char* path, uint16_t dataseg, uint16_t dataoffset, uint16_t size)
{
	FILE* fp = fopen(path, "rb");
	
	int ch = fgetc(fp);
	if(ch == EOF) return 0;
	
	while(ch != EOF)
	{
		*FAR_BYTE_PTR(dataseg, dataoffset) = (uint8_t)ch;
		ch = fgetc(fp);
		dataoffset++;
	}
	fclose(fp);
	
	return 1; 
}

static uint16_t LoadComProgram(const char* path, CPU_Context* pcontext, uint16_t* pProgIP)
{
	uint16_t _prgSeg = NEXT_SEG(GET_CS());
	*pProgIP = 0x100;
	
	if(!loadCode(path, _prgSeg, *pProgIP, 65535))
		return 0;
	
	pcontext->AX = 0;
	pcontext->CX = 0;
	pcontext->DX = 0;
	pcontext->BX = 0;
	pcontext->SP = 0;
	pcontext->BP = 0;
	pcontext->SI = 0;
	pcontext->DI = 0;
	pcontext->DS = _prgSeg;
	pcontext->ES = _prgSeg;
	pcontext->SS = _prgSeg;
	
	//push 0
	pcontext->SP -= 2;
	*((uint16_t*)pcontext->SP) = 0;
	
	return _prgSeg;
}

static uint16_t LoadExeProgram(const char* path, CPU_Context* pcontext, uint16_t* pProgIP)
{
	return 0;
} 

uint16_t LoadProgram(const char* path, CPU_Context* pcontext, uint16_t* pProgIP)
{
	char* progType = strchr(path, '.');
	if(!progType)
	{
		errno = ENOEXEC;
		return 0;
	}
	
	progType++;
	
	if(!strcmp(progType, "com")) return LoadComProgram(path, pcontext, pProgIP);
	else if(!strcmp(progType, "exe")) return LoadExeProgram(path, pcontext, pProgIP);
	else
	{
		errno = ENOEXEC;
		return 0;
	}
}

void FreeProgram(uint16_t seg)
{
	//_dos_freemem(seg);
}
