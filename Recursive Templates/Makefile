CC=g++

CFLAGS=-O0 -g

MAIN_SOURCES=main.cpp
PLAY_SOURCES=playground.cpp
BETTER_SOURCES=better.cpp

LOGFILE=logs
EXECUTABLE=test
 
all: compile run

play: compile_play run

better: compile_better run

erase:
	clear

compile:
	${CC} ${CFLAGS} $(MAIN_SOURCES) -o $(EXECUTABLE)

compile_play:
	${CC} ${CFLAGS} $(PLAY_SOURCES) -o $(EXECUTABLE)

compile_better:
	${CC} ${CFLAGS} $(BETTER_SOURCES) -o $(EXECUTABLE)

run:
	./${EXECUTABLE} > logs

clean:
	@rm -rf ${EXECUTABLE}