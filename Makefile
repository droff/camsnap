all:
	gcc -fPIC -Wall -shared -I include -O2 src/camsnap.c -o libcamsnap.so

clean:
	$(RM) libcamsnap.so
