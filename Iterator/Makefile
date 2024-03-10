CC=g++

CFLAGS=-O0 -g -std=c++23
SANITIZER=-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

MAIN_SOURCES=main.cpp

LOGFILE=logs
READ_FROM=data.txt
EXECUTABLE=test
 
all: compile run

erase:
	clear

compile:
	${CC} ${CFLAGS} ${SANITIZER} $(MAIN_SOURCES) -o $(EXECUTABLE)

run:
	./${EXECUTABLE} < ${READ_FROM} > ${LOGFILE}

clean:
	@rm -rf ${EXECUTABLE}