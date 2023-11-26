#! /bin/bash

App_run: AppFunc.o init.o MainMenu.o openFunc.o releaseFunc.o exitFunc.o writeData.o readData.o
	gcc -o App_run AppFunc.o init.o MainMenu.o openFunc.o releaseFunc.o exitFunc.o writeData.o readData.o

readData.o: readData.c header.h decleration.h datatype.h
	gcc -c readData.c
writeData.o: writeData.c header.h decleration.h datatype.h
	gcc -c writeData.c
exitFunc.o: exitFunc.c header.h decleration.h datatype.h
	gcc -c exitFunc.c
releaseFunc.o: releaseFunc.c header.h decleration.h datatype.h
	gcc -c releaseFunc.c
openFunc.o: openFunc.c header.h decleration.h datatype.h
	gcc -c openFunc.c
MainMenu.o: MainMenu.c header.h decleration.h datatype.h
	gcc -c MainMenu.c
init.o: init.c header.h decleration.h datatype.h
	gcc -c init.c
AppFunc.o: AppFunc.c header.h decleration.h datatype.h
	gcc -c AppFunc.c

clean:
	rm *.o
