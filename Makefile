CC=gcc
INC=-I./include
LIB=-L./lib -lf1goal
CFLAG=-g -O2 -Werror
TOOLS=fmore secure_log ssh_hack_defend

fmore:fmore.o
	${CC} -o $@ $^ ${LIB}
ssh_hack_defend:ssh_hack_defend.o
	${CC} -o $@ $^ ${LIB}
secure_log:secure_log.o
	${CC} -o $@ $^ ${LIB}

%.o:src/%.c
	${CC} ${CFLAG} -c $< ${INC}

.PHONY:clean all

all:${TOOLS}

clean:
	rm -f *.o
	rm -f ${TOOLS}
