all: main.c
	gcc -g -Wall -o camsnap main.c camsnap.c

clean:
	$(RM) camsnap

