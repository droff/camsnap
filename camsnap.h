#ifndef CAMSNAP_H
#define CAMSNAP_H

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

/*
   * initialize v4l2_buffer
   *
   * fd: file descriptor
   * buffer: buffer information
   *
   * returns: new buffer
 */
char *camsnap_buffer( int *fd, struct v4l2_buffer *buffer );

/*
   * initialize device settings
   * MJPG only!
   *
   * returns: file descriptor
 */
int camsnap_init( struct v4l2_capability *capability, 
		  struct v4l2_format *format,
		  struct v4l2_requestbuffers *rb,
		  struct v4l2_buffer *buffer,
		  unsigned short width,
		  unsigned short height );

/*
   * capturing from device into buffer
   *
   * fd: file descriptor
   * buffer: buffer
   * rb: buffer information
   *
   * returns: 0
 */
int camsnap_start( int *fd, 
		   struct v4l2_buffer *buffer,
		   struct v4l2_requestbuffers *rb );

/*
   * save into file
   *
   * filename: file name
   * membuffer: raw data - result of camsnap_start
   * length: buffer length
   *
   * returns: 0
 */
int camsnap_save( const char *filename,
		  char *membuffer,
		  int length );

/*
   * free data
   *
   * returns: 0
 */
int camsnap_close( int *fd,
		   struct v4l2_capability *cap, 
		   struct v4l2_format *format, 
		   struct v4l2_requestbuffers *rb,
		   struct v4l2_buffer *buffer );

/*
   * init device and buffers, take snapshot
   *
   * width: image width
   * height: image height
   * buffer_size: buffer size
   *
   * returns: raw data
 */
char *camsnap_shot( unsigned short width,
		    unsigned short height,
		    int *buffer_size );

#endif
