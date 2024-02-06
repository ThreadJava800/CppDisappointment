CC=g++

CFLAGS=-fno-elide-constructors -O0 -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr")


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