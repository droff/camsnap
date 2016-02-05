#include "camsnap.h"

int main( int argc, char **argv )
{
/*	struct v4l2_capability *cap 		= malloc(sizeof(struct v4l2_capability));
	struct v4l2_format *format 		= malloc(sizeof(struct v4l2_format));
	struct v4l2_requestbuffers *rb		= malloc(sizeof(struct v4l2_requestbuffers));
	struct v4l2_buffer *buffer 		= malloc(sizeof(struct v4l2_buffer));

	int fd = camsnap_init(cap, format, rb, buffer, 1280, 720);
	char *membuffer = camsnap_buffer(&fd, buffer);

	camsnap_start(&fd, buffer, rb);
	camsnap_save(argv[1], membuffer, buffer);
	camsnap_close(&fd, cap, format, rb, buffer);
*/
	char *filename = NULL;

	if (argc != 2)
		filename = "./shot.jpg\0";
	else
		filename = argv[1];

	int buffer_size = 0;
	char *buffer = camsnap_shot(&buffer_size);
	camsnap_save(filename, buffer, buffer_size);

	printf("%d bytes saved\n", buffer_size);

	return 0;
}
