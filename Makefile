PROJ=compiler
EXE=${PROJ}.exe
SRC=${PROJ}.cpp
${EXE}:${SRC}
	g++ -g ${SRC} -o ${EXE}

run:${EXE}
	./${EXE} sort.txt sort.exe

test:${EXE}
	./${EXE} test

debug:${EXE}
	gdb --args ${EXE} sort.txt sort.exe