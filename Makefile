CC=gcc
INC=-I./include
LIB=-L./lib -lf1goal -lpthread
CFLAG=-g -O2 -Werror
TOOLS=fmore secure_log ssh_hack_defend get_addrinfo forbid_ip GoU show_dir gospawn ston schar2int

ston:ston.o lib/libf1goal.a
	${CC} -o $@ $^ 
fmore:fmore.o
	${CC} -o $@ $^ ${LIB}
ssh_hack_defend:ssh_hack_defend.o
	${CC} -o $@ $^ ${LIB}
secure_log:secure_log.o
	${CC} -o $@ $^ ${LIB}

test_addrinfo:get_addrinfo.o
	${CC} -o $@ $^ ${LIB}

forbid_ip:forbid_ip.o
	${CC} -o $@ $^ ${LIB}

GoU:gou.o lib/libf1goal.a
	${CC} -o $@ $^ -lpthread

show_dir:show_dir.o
	${CC} -o $@ $^ ${LIB}

schar2int: schar_to_int.o
	${CC} -o $@ $^

gospawn:gospawn.o lib/libf1goal.a
	${CC} -o $@ $^ -lpthread

%.o:src/%.c
	${CC} ${CFLAG} -c $< ${INC}

.PHONY:clean all

all:${TOOLS}

clean:
	rm -f *.o
	rm -f ${TOOLS}
