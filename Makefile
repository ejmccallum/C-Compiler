PROJ=compiler
EXE=${PROJ}.exe
SRC=${PROJ}.cpp
HEADERS=*.hpp
${EXE}:${SRC} ${HEADERS}
	g++ -g ${SRC} -o ${EXE}

run:${EXE}
	./${EXE} sort.txt sort.s
	g++ sort.s -o sort.exe
	./sort.exe

test:${EXE}
	./${EXE} test

debug:${EXE}
	gdb --args ${EXE} sort.txt sort.exe

clean:
	rm -f ${EXE} *.o *.exe *.stackdump

all: clean ${EXE} run