CC=gcc
EXECS=schar2int

schar2int:schar_to_int.o
	${CC} -o $@ $^

%.o:%.c
	${CC} -c $<

.PHONY:clean all

all: ${EXECS}

clean:
	rm -f *.o
	rm -f ${EXECS}
