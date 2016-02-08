all:
	gcc -c -Wall -Werror -fpic camsnap.c
	gcc -shared -o libcamsnap.so camsnap.o

clean:
	$(RM) libcamsnap.so camsnap.o
