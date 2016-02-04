#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <fcntl.h>

char *camsnap_buffer(int *, struct v4l2_buffer *);

int camsnap_init( struct v4l2_capability *capability, 
		  struct v4l2_format *format,
		  struct v4l2_requestbuffers *rb,
		  struct v4l2_buffer *buffer,
		  unsigned short width,
		  unsigned short height );

int camsnap_start( int *fd, 
		   struct v4l2_buffer *buffer,
		   struct v4l2_requestbuffers *rb );

int camsnap_save( const char *filename,
		  char *membuffer,
		  struct v4l2_buffer *buffer );

int camsnap_close( int *fd,
		   struct v4l2_capability *cap, 
		   struct v4l2_format *format, 
		   struct v4l2_requestbuffers *rb,
		   struct v4l2_buffer *buffer );

