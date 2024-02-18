CC=g++

CFLAGS=-O0 -g

SOURCES= main.cpp
LOGFILE=logs
EXECUTABLE=test
 
all: compile run

erase:
	clear

compile:
	${CC} ${CFLAGS} $(SOURCES) -o $(EXECUTABLE)

run:
	./${EXECUTABLE} > logs

clean:
	@rm -rf ${EXECUTABLE}