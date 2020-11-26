#ifndef __IMAGE_H__
#define __IMAGE_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum { RGB, RGBA, ARGB, ALPHA, LUMINANCE, LUMINANCE_ALPHA} format_t;
typedef enum { UNSIGNED_INT_8_8_8_8, UNSIGNED_BYTE } type_t;

typedef struct image {
	int width;
	int height;
	format_t format;
	type_t type;
    int alignment;
	unsigned char* pixels;
} image_t;
	
#ifdef __cplusplus
}
#endif
	
#endif