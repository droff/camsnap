#include "camsnap.h"

char *camsnap_buffer( int fd, struct v4l2_buffer *buffer )
{
	char* membuffer = mmap( NULL, buffer->length, PROT_READ | PROT_WRITE, 
				MAP_SHARED, fd, buffer->m.offset );
	 
	if (membuffer == MAP_FAILED) {
		perror("mmap");
		return NULL;
	}
	 
	memset(membuffer, 0, buffer->length);

	return membuffer;
}

int camsnap_init( struct v4l2_format *format,
		  struct v4l2_requestbuffers *rb,
		  struct v4l2_buffer *buffer,
		  const char *device,
		  unsigned short width,
		  unsigned short height )
{
	struct v4l2_capability capability = {};

	// open device
	int fd = open(device, O_RDWR);

	// check capabilities
	if (ioctl(fd, VIDIOC_QUERYCAP, &capability) < 0) {
		perror("VIDIOC_QUERYCAP");
		return -1;
	}

	if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "The device isn't compatible for capture!\n");
		return -1;
	}

	// set format
	// MJPEG
	format->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format->fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	format->fmt.pix.field = V4L2_FIELD_NONE;
	format->fmt.pix.width = width;
	format->fmt.pix.height = height;

	if (ioctl(fd, VIDIOC_S_FMT, format) == -1) {
		perror("VIDIOC_S_FMT");
		return -1;
	}

	// set buffers
	rb->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	rb->memory = V4L2_MEMORY_MMAP;
	rb->count = 1;
	 
	if (ioctl(fd, VIDIOC_REQBUFS, rb) == -1) {
		perror("VIDIOC_REQBUFS");
		return -1;
	}

	// allocate buffers
	memset(buffer, 0, sizeof(*buffer));
	 
	buffer->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buffer->memory = V4L2_MEMORY_MMAP;
	buffer->index = 0;
	 
	if (ioctl(fd, VIDIOC_QUERYBUF, buffer) == -1) {
		perror("VIDIOC_QUERYBUF");
		return -1;
	}

	return fd;
}

int camsnap_start( int fd, 
		   struct v4l2_buffer *buffer,
		   struct v4l2_requestbuffers *rb )
{
	int type = buffer->type;

	if (ioctl(fd, VIDIOC_STREAMON, &type) < 0) {
		perror("VIDIOC_STREAMON");
		return -1;
	}

	if (ioctl(fd, VIDIOC_QBUF, buffer) < 0) {
		perror("VIDIOC_QBUF");
		return -1;
	}

	if (ioctl(fd, VIDIOC_DQBUF, buffer) < 0) {
		perror("VIDIOC_DQBUF");
		return -1;
	}
	
	if (ioctl(fd, VIDIOC_STREAMOFF, &type) < 0) {
		perror("VIDIOC_STREAMOFF");
		return -1;
	}

	return 0;
}

int camsnap_save( const char *filename,
		  char *membuffer,
		  int length )
{
	size_t wbytes = 0;
	int file = open(filename, O_WRONLY | O_CREAT, 0660);

	if (file < 0) {
		perror("open");
		return 1;
	}

	wbytes = write(file, membuffer, length);
	close(file);

	return wbytes;
}

char *camsnap_shot( const char *device,
		    unsigned short width,
		    unsigned short height,
		    int *buffer_size )
{	
	struct v4l2_format *format 		= malloc(sizeof(struct v4l2_format));
	struct v4l2_requestbuffers *rb		= malloc(sizeof(struct v4l2_requestbuffers));
	struct v4l2_buffer *buffer 		= malloc(sizeof(struct v4l2_buffer));
	char *jpeg 				= malloc(sizeof(char));

	int fd = camsnap_init(format, rb, buffer, device, width, height);
	char *membuffer = camsnap_buffer(fd, buffer);

	camsnap_start(fd, buffer, rb);
	*buffer_size = mjpeg2jpeg(membuffer, buffer->length, jpeg);
	
	close(fd);

	munmap(membuffer, buffer->length);
	free(format);
	free(rb);
	free(buffer);

	return jpeg;
}

int mjpeg2jpeg( const char *buffer, 
		int buffer_size,
		char *data )
{
	int i, j, size = 0;
	int len = (buffer[4] << 8) | buffer[5];

	for ( i = 0; i < buffer_size; i++ ) {
		data[i+j] = buffer[i];
		
		if ( i == len + 3 )
			for ( j = 0; j < (sizeof(fixed_dht) / sizeof(char)) ; j++ )
				data[i+1+j] = fixed_dht[j];
		
		if ( (buffer[i] == 0xff) && (buffer[i+1] == 0xd9) ) {
			data[i+j+1] = buffer[i+1];
			size = i + j + 2;
			break;
		}
	}

	return size;
}
