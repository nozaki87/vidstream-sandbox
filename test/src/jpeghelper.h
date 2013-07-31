#ifndef JPEGHELPER_H
#define JPEGHELPER_H

extern unsigned char  *load_jpg(char const *filename);
extern unsigned char  *load_memory_jpg(char *memory, int size);
extern void mjpeg2jpeg(unsigned char * dst, unsigned char * src, int srcsize);

#endif // JPEGHELPER_H
