#include <stdio.h>
#include "..\..\hdrs\context.h"
#include "..\..\hdrs\ints.h"
#include "context_stable.h"
#include "context_test.h"

Stable_CPU_Context stable_context_func;
Stable_CPU_Context stable_context;
CPU_Context context;

void Test_SAVE_CONTEXT()
{
	STABLE_SAVE_CONTEXT(&stable_context_func);
	
	stable_context = {1,2,3,4,5,6,7,8,9,10,11};
	STABLE_LOAD_CONTEXT(&stable_context);
	SAVE_CONTEXT(&context);
	
	STABLE_LOAD_CONTEXT(&stable_context_func);
	
	if(context == stable_context)
		printf("[Test] SaveContext Passed");
	else
		printf("[Test] SaveContext NOT Passed");
}

void Test_LOAD_CONTEXT()
{
	STABLE_SAVE_CONTEXT(&stable_context_func);
	
	context = {1,2,3,4,5,6,7,8,9,10,11};
	
	LOAD_CONTEXT(&context);
	STABLE_SAVE_CONTEXT(&stable_context);
	
	STABLE_LOAD_CONTEXT(&stable_context_func);
	
	if(context == stable_context)
		printf("[Test] LoadContext Passed");
	else
		printf("[Test] LoadContext NOT Passed");
}

void Test_context()
{
	Test_SAVE_CONTEXT();
	Test_LOAD_CONTEXT();
}

int main()
{
	Test_context();
	
	return 0;
}