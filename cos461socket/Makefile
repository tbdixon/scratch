default: c

c: client-c server-c

client-c: client-c.c port-utils-c.o
	gcc -Wall -pedantic -o client-c client-c.c port-utils-c.o

server-c: server-c.c port-utils-c.o
	gcc -Wall -pedantic -o server-c server-c.c port-utils-c.o

port-utils-c.o: port-utils-c.c
	gcc -Wall -pedantic -c port-utils-c.c 

clean:
	rm -f server-c client-c port-utils-c.o
