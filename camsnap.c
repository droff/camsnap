#include "camsnap.h"

// make buffer
char *camsnap_buffer(int *fd, struct v4l2_buffer *buffer)
{
	char* membuffer = mmap( NULL, buffer->length, PROT_READ | PROT_WRITE, 
				MAP_SHARED, *fd, buffer->m.offset );
	 
	if (membuffer == MAP_FAILED) {
		perror("mmap");
		return NULL;
	}
	 
	memset(membuffer, 0, buffer->length);

	return membuffer;
}

// camsnap_init check capabilities and returns descriptor
int camsnap_init( struct v4l2_capability *capability, 
		  struct v4l2_format *format,
		  struct v4l2_requestbuffers *rb,
		  struct v4l2_buffer *buffer,
		  unsigned short width,
		  unsigned short height )
{
	// open device
	int fd = open("/dev/video0", O_RDWR);

	// check capabilities
	if (ioctl(fd, VIDIOC_QUERYCAP, capability) < 0) {
		perror("VIDIOC_QUERYCAP");
		return -1;
	}

	if (!(capability->capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "The device isn't compatible for capture!\n");
		return -1;
	}

	// set format
	format->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format->fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	format->fmt.pix.width = width;
	format->fmt.pix.height = height;

	if (ioctl(fd, VIDIOC_S_FMT, format) < 0) {
		perror("VIDIOC_S_FMT");
		return -1;
	}

	// set buffers
	rb->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	rb->memory = V4L2_MEMORY_MMAP;
	rb->count = 1;
	 
	if (ioctl(fd, VIDIOC_REQBUFS, rb) < 0) {
		perror("VIDIOC_REQBUFS");
		return -1;
	}

	// allocate buffers
	memset(buffer, 0, sizeof(*buffer));
	 
	buffer->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buffer->memory = V4L2_MEMORY_MMAP;
	buffer->index = 0;
	 
	if (ioctl(fd, VIDIOC_QUERYBUF, buffer) < 0) {
		perror("VIDIOC_QUERYBUF");
		return -1;
	}

	return fd;
}

// camsnap_start capturing
int camsnap_start( int *fd, 
		   struct v4l2_buffer *buffer,
		   struct v4l2_requestbuffers *rb )
{
	int type = buffer->type;

	if (ioctl(*fd, VIDIOC_STREAMON, &type) < 0) {
		perror("VIDIOC_STREAMON");
		return -1;
	}

	if (ioctl(*fd, VIDIOC_QBUF, buffer) < 0) {
		perror("VIDIOC_QBUF");
		return -1;
	}

	if (ioctl(*fd, VIDIOC_DQBUF, buffer) < 0) {
		perror("VIDIOC_QBUF");
		return -1;
	}

	for (int i = 0; i < rb->count; i++) {
		buffer->index = i;
	}

	if (ioctl(*fd, VIDIOC_STREAMOFF, &type) < 0) {
		perror("VIDIOC_STREAMOFF");
		return -1;
	}

	return 0;
}

int camsnap_save( const char *filename,
		  char *membuffer,
		  struct v4l2_buffer *buffer )
{
	int file = open(filename, O_WRONLY | O_CREAT, 0660);

	if (file < 0) {
		perror("open");
		return 1;
	}

	write(file, membuffer, buffer->length);
	close(file);

	return 0;
}

int camsnap_close( int *fd,
		   struct v4l2_capability *cap, 
		   struct v4l2_format *format, 
		   struct v4l2_requestbuffers *rb,
		   struct v4l2_buffer *buffer )
{
	free(cap);
	free(format);
	free(rb);
	free(buffer);
	close(*fd);

	return 0;
}

