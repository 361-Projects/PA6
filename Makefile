# ******************************************************
# PROGRAM: User-Level Thread Library                   *
# CLASS: CISC 361-011                                  *
# AUTHORS:                                             *
#    Alex Sederquest | alexsed@udel.edu | 702414270    *
#    Ben Segal | bensegal@udel.edu | 702425559         *
# ******************************************************

CC = gcc
CFLAGS = -g

LIBOBJS = t_lib.o 

TSTOBJS = test*.o

HWOBJS = rendezvous.o order.o

# specify the executable 

EXECS = test?? rendezvous order 

# specify the source files

LIBSRCS = t_lib.c

TSTSRCS = test00.c

# ar creates the static thread library

t_lib.a: ${LIBOBJS} Makefile
	ar rcs t_lib.a ${LIBOBJS}

# here, we specify how each file should be compiled, what
# files they depend on, etc.

t_lib.o: t_lib.c t_lib.h Makefile
	${CC} ${CFLAGS} -c t_lib.c

test00.o: test00.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c test00.c

t00: test00
test00: test00.o t_lib.a Makefile
	${CC} ${CFLAGS} test00.o t_lib.a -o test00

test01.o: test01.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c test01.c

t01: test01
test01: test01.o t_lib.a Makefile
	${CC} ${CFLAGS} test01.o t_lib.a -o test01

test03.o: test03.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c test03.c

t03: test03
test03: test03.o t_lib.a Makefile
	${CC} ${CFLAGS} test03.o t_lib.a -o test03


rendezvous: rendezvous.o rendezvous.c
	gcc rendezvous.o -o rendezvous t_lib.a

rendezvous.o: rendezvous.c t_lib.a
	gcc -c t_lib.a rendezvous.c

order: order.o order.c
	gcc order.o -o order t_lib.a

order.o: order.c t_lib.a
	gcc -c t_lib.a order.c
	
hw: rendezvous order

clean:
	rm -f t_lib.a ${EXECS} ${LIBOBJS} ${TSTOBJS} ${HWOBJS}
