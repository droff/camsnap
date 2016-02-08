all: main.c
	gcc -g -Wall -o camsnap main.c camsnap.c

library:
	gcc -c -Wall -Werror -fpic camsnap.c
	gcc -shared -o libcamsnap.so camsnap.o

clean:
	$(RM) camsnap libcamsnap.so camsnap.o

