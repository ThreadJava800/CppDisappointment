CC=g++

CFLAGS=-O0 -g -std=c++23

MAIN_SOURCES=main.cpp

LOGFILE=logs
EXECUTABLE=test
 
all: compile run

erase:
	clear

compile:
	${CC} ${CFLAGS} $(MAIN_SOURCES) -o $(EXECUTABLE)

run:
	./${EXECUTABLE} > ${LOGFILE}

clean:
	@rm -rf ${EXECUTABLE}