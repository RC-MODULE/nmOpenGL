//------------------------------------------------------------------------
//  Demostration for MAX 2017 
//
// 
//
//  Copyright (c) 2017 RC Module Inc.
//------------------------------------------------------------------------

#include "vshell.h"
#include "demo3d_common.h"
#include "demo3d_host.h"
#include "service.h"
#include "imagebuffer.h"

extern NMGL_FrameBuffer *remote;
void readColorBackNM(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);
void readColorFrontNM(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);
bool frameBufferIsEmpty(NMGL_FrameBuffer *remoteAddr);
void frameBufferIncTail(NMGL_FrameBuffer *remoteAddr);


int nmglvsHostReadImage(int* dstImage)
{
	S_VS_MouseStatus mouseStatus;
	VS_GetMouseStatus(&mouseStatus);


	while (frameBufferIsEmpty(remote));

	if (mouseStatus.nKey != VS_MOUSE_LBUTTON)
		readColorBackNM(dstImage, remote, 0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);

	frameBufferIncTail(remote);

	return 0;
};
