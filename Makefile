ifeq ($(OS),Windows_NT)
	exe_name = dracofuck.exe
	rm_command = del /F /S /Q
else
	exe_name = dracofuck
	rm_command = rm -rf
endif

all: dracofuck

dracofuck: main.o buffer_mgt.o
	gcc -o $(exe_name) main.o buffer_mgt.o

main.o: main.c buffer_mgt.h struct_boucle.h struct_buffer_element.h
	gcc -o main.o -c main.c

buffer_mgt.o: buffer_mgt.c struct_boucle.h struct_buffer_element.h
	gcc -o buffer_mgt.o -c buffer_mgt.c

clean:
	$(rm_command) *.o

mrproper: clean
	$(rm_command) $(exe_name)
