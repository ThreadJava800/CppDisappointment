CC=g++

CFLAGS=-fno-elide-constructors -O0


SOURCES=main.cpp
EXECUTABLE=test
 
all: compile run

erase:
	clear

compile:
	@${CC} ${CFLAGS} $(SOURCES) -o $(EXECUTABLE)

run:
	@./${EXECUTABLE}

clean:
	@rm -rf ${EXECUTABLE}