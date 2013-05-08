CC=gcc
BIN=t_sort
INC=-I./
LIB=
CFLAGS=-g
LFLAGS=

t_sort:t_sort.o sort.o
	${CC} -o $@ $^ ${LIB}	

%.o:%.c
	${CC} ${CFLAGS} -c $< ${INC}

.PHONY:clean all

all:${BIN}

clean:
	rm -f *.o ${BIN}
