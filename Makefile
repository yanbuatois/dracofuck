all: dracofuck

dracofuck: main.o buffer_mgt.o
	gcc -o dracofuck main.o buffer_mgt.o

main.o: main.c buffer_mgt.h struct_boucle.h
	gcc -o main.o -c main.c

buffer_mgt.o: buffer_mgt.c struct_boucle.h
	gcc -o buffer_mgt.o -c buffer_mgt.c

clean:
	rm -rf *.o

mrproper:
	rm -rf dracofuck
