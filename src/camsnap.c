#include "camsnap.h"

int camsnap_save( const char *filename,
		  byte *buffer,
		  int length )
{
	int file = open(filename, O_WRONLY | O_CREAT, 0660);

	if (file < 0) {
		perror("open");
		return -1;
	}

	size_t wbytes = write(file, buffer, length);
	close(file);

	return wbytes;
}

int camsnap_shot( const char *device,
		  unsigned short width,
		  unsigned short height,
		  byte *jpeg )
{	
	struct v4l2_format format 	  = {};
	struct v4l2_requestbuffers rb	  = {};
	struct v4l2_buffer buffer 	  = {};
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
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	format.fmt.pix.field = V4L2_FIELD_NONE;
	format.fmt.pix.width = width;
	format.fmt.pix.height = height;

	if (ioctl(fd, VIDIOC_S_FMT, &format) == -1) {
		perror("VIDIOC_S_FMT");
		return -1;
	}

	// set buffers
	rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	rb.memory = V4L2_MEMORY_MMAP;
	rb.count = 1;
	 
	if (ioctl(fd, VIDIOC_REQBUFS, &rb) == -1) {
		perror("VIDIOC_REQBUFS");
		return -1;
	}
	 
	buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buffer.memory = V4L2_MEMORY_MMAP;
	buffer.index = 0;
	 
	if (ioctl(fd, VIDIOC_QUERYBUF, &buffer) == -1) {
		perror("VIDIOC_QUERYBUF");
		return -1;
	}

	byte *membuffer = mmap( NULL, buffer.length, PROT_READ | PROT_WRITE, 
				MAP_SHARED, fd, buffer.m.offset );
	 
	if (membuffer == MAP_FAILED) {
		perror("mmap");
		return -1;
	}
	 
	memset(membuffer, 0, buffer.length);
	int type = buffer.type;

	if (ioctl(fd, VIDIOC_STREAMON, &type) < 0) {
		perror("VIDIOC_STREAMON");
		return -1;
	}

	if (ioctl(fd, VIDIOC_QBUF, &buffer) < 0) {
		perror("VIDIOC_QBUF");
		return -1;
	}

	if (ioctl(fd, VIDIOC_DQBUF, &buffer) < 0) {
		perror("VIDIOC_DQBUF");
		return -1;
	}
	
	if (ioctl(fd, VIDIOC_STREAMOFF, &type) < 0) {
		perror("VIDIOC_STREAMOFF");
		return -1;
	}

	/* dht + buffer payload = jpeg  */
	int i = 0, j = 0, jpeg_size = 0;
	int len = (membuffer[4] << 8) | membuffer[5];

	for ( i = 0; i < buffer.length; i++ ) {
		jpeg[i+j] = membuffer[i];
		
		if ( i == len + 3 )
			for ( j = 0; j < (sizeof(fixed_dht) / sizeof(byte)) ; j++ )
				jpeg[i+j+1] = fixed_dht[j];
		
		if ( (membuffer[i] == 0xff) && (membuffer[i+1] == 0xd9) ) {
			jpeg[i+j+1] = membuffer[i+1];
			jpeg_size = i + j + 2;
			break;
		}
	}
	
	close(fd);
	munmap(membuffer, buffer.length);

	return jpeg_size;
}

