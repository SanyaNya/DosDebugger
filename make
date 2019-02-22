MAIN_HDRS=hdrs\ints.h hdrs\context.h hdrs\dbgr.h hdrs\prgldr.h
PRGLDR_HDRS=hdrs\ints.h hdrs\context.h hdrs\prgldr.h
DBGR_HDRS=hdrs\ints.h hdrs\context.h hdrs\dbgr.h
CONTEXT_HDRS=hdrs\context.h

gen.com: objs\main.obj objs\prgldr.obj objs\dbgr.obj objs\context.obj
	wcl -0 -lr -l=COM objs\main.obj objs\prgldr.obj objs\dbgr.obj objs\context.obj -fe=gen.com -fr=errs\link.err

objs\main.obj: src\main.c $(MAIN_HDRS)
	wcc -0 -ox src\main.c -fo=objs\main.obj -fr=errs\main.err

objs\prgldr.obj: src\prgldr.c $(PRGLDR_HDRS)
	wcc -0 -ox src\prgldr.c -fo=objs\prgldr.obj -fr=errs\prgldr.err

objs\dbgr.obj: src\dbgr.c $(DBGR_HDRS)
	wcc -0 -ox src\dbgr.c -fo=objs\dbgr.obj -fr=errs\dbgr.err
	
objs\context.obj: src\context.c $(CONTEXT_HDRS)
	wcc -0 src\context.c -fo=objs\context.obj -fr=errs\context.err