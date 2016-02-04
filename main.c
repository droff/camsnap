#include "camsnap.h"

int main(int argc, char *argv[])
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
	char *filename = argv[1];

	if (filename == NULL)
		*filename = "./shot.jpg";

	char *buffer = Perform(argv[1]);
	printf("%i bytes saved\n", strlen(buffer));

	return 0;
}
