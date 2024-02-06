CC=g++

CFLAGS=-fno-elide-constructors -O0


SOURCES=main.cpp
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