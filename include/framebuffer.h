#pragma once
#include "nmpp.h"
#include "nmgl.h"


#define MAX_BUFFERS_COUNT 4
#define MAX_CONNECTOR_COUNT 2

struct NMGL_Framebuffer {
	//порядок важен
	volatile unsigned long long head;
	volatile unsigned long long tail;
	int sizeOfAddr;
	int sizeOfInt;
	int width;
	int height;
	void *buffers[MAX_BUFFERS_COUNT];
};

typedef void (*NMGL_FrameReadColorFunc)(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
typedef void (*NMGL_FrameWriteColorFunc)(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
typedef void (*NMGL_FrameReadDepthFunc)(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
typedef void (*NMGL_FrameWriteDepthFunc)(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
typedef void (*NMGL_FrameReadStencilFunc)(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
typedef void (*NMGL_FrameWriteStencilFunc)(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);

typedef int (*NMGL_GetFrameWidth)(NMGL_Framebuffer *fb);
typedef int (*NMGL_GetFrameHeight)(NMGL_Framebuffer *fb);
typedef void (*NMGL_SetFrameSize)(NMGL_Framebuffer *fb, int width, int height);


void NMGL_FramebufferInit(NMGL_Framebuffer *fb, int width, int height);


struct NMGL_FramebufferConnector{
	NMGL_Framebuffer *remoteAddr;

	NMGL_FrameReadColorFunc readColor;
	NMGL_FrameWriteColorFunc writeColor;

	NMGL_FrameReadDepthFunc readDepth;
	NMGL_FrameWriteDepthFunc writeDepth;

	NMGL_FrameReadStencilFunc readStencil;
	NMGL_FrameWriteStencilFunc writeStencil;

	NMGL_GetFrameWidth getWidth;
	NMGL_GetFrameHeight getHeight;
	NMGL_SetFrameSize setSize;
};

