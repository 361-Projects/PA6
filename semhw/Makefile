all: fork-join rendezvous order

fork-join: fork-join.o fork-join.c
	gcc fork-join.o -o fork-join -pthread

fork-join.o: fork-join.c
	gcc -c -pthread fork-join.c

rendezvous: rendezvous.o rendezvous.c
	gcc rendezvous.o -o rendezvous -pthread

rendezvous.o: rendezvous.c
	gcc -c -pthread rendezvous.c

order: order.o order.c
	gcc order.o -o order -pthread

order.o: order.c
	gcc -c -pthread order.c

clean:
	rm *.o fork-join rendezvous order