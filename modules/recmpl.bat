del main.err
del main.obj
del gen.com

cd context
call recmpl.bat
cd ..

cd dbgr
call recmpl.bat
cd ..

cd prgldr
call recmpl.bat
cd ..

wmake -f make