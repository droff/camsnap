#include "../camsnap.h"

int main( int argc, char **argv )
{
	char *filename = NULL;
	int buffer_size = 0;

	if (argc != 2)
		filename = "./shot.jpg\0";
	else
		filename = argv[1];

	char *buffer = camsnap_shot("/dev/video0", 1280, 720, &buffer_size);
	camsnap_save(filename, buffer, buffer_size);
	camsnap_free(buffer, buffer_size);

	printf("%d bytes saved\n", buffer_size);

	return 0;
}
