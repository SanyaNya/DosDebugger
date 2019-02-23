#include <stdio.h>
#include <string.h>
#include "..\..\hdrs\context.h"
#include "ints.h"
#include "scontext.h"
#include "tcontext.h"

static Stable_CPU_Context stable_context_func;
static Stable_CPU_Context stable_context = {1,2,3,4,5,6,7,8,9,10,11,12};
static CPU_Context context = {1,2,3,4,5,6,7,8,9,10,11,12};

int Test_SAVE_CONTEXT()
{
	STABLE_SAVE_CONTEXT(&stable_context_func);
	
	STABLE_LOAD_CONTEXT(&stable_context);
	SAVE_CONTEXT(&context);
	
	STABLE_LOAD_CONTEXT(&stable_context_func);
	
	if(!memcmp(&context, &stable_context, sizeof(CPU_Context)))
	{
		printf("  [Test] SaveContext Passed\n");
		return 1;
	}
	else
	{
		printf("  [Test] SaveContext NOT Passed\n");
		return 0;
	}
}
int Test_LOAD_CONTEXT()
{
	STABLE_SAVE_CONTEXT(&stable_context_func);
	
	LOAD_CONTEXT(&context);
	STABLE_SAVE_CONTEXT(&stable_context);
	
	STABLE_LOAD_CONTEXT(&stable_context_func);
	
	if(!memcmp(&context, &stable_context, sizeof(CPU_Context)))
	{
		printf("  [Test] LoadContext Passed\n");
		return 1;
	}
	else
	{
		printf("  [Test] LoadContext NOT Passed\n");
		return 0;
	}
}

void Test_context()
{
	int total = 0;
	
	printf("[CONTEXT] Run tests:\n");
	
	total += Test_SAVE_CONTEXT();
	total += Test_LOAD_CONTEXT();
	
	printf("[CONTEXT] Total: %i/2\n", total);
}

int main()
{
	Test_context();
	
	return 0;
}
