all: main.c
	gcc --std=c99 -g -Wall -o camsnap main.c camsnap.c

clean:
	$(RM) camsnap

