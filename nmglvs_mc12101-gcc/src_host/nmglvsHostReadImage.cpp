//------------------------------------------------------------------------
//  Demostration for MAX 2017 
//
// 
//
//  Copyright (c) 2017 RC Module Inc.
//------------------------------------------------------------------------

#include "VShell.h"
#include "demo3d_common.h"
#include "service.h"
#include "imagebuffer.h"


extern ImageConnector hostImageRB;
int tempImage[WIDTH_IMAGE * HEIGHT_IMAGE];

int nmglvsHostReadImage(int* dstImage)
{
	S_VS_MouseStatus mouseStatus;
	VS_GetMouseStatus(&mouseStatus);
#if defined(PROFILER0) || defined(PROFILER1)
	if (mouseStatus.nKey == VS_MOUSE_LBUTTON) {
#else
	if (mouseStatus.nKey != VS_MOUSE_LBUTTON) {
#endif

#ifdef  OUTPUT_IMAGE_RGB565
		hostImageRB.pop((NMGL_IMAGE*)tempImage, 1);
		convertRGB565_RGB8888((rgb565*)tempImage, (rgb8888*)dstImage, WIDTH_IMAGE * HEIGHT_IMAGE);
#endif //  OUTPUT_IMAGE_RGB565
#ifdef  OUTPUT_IMAGE_RGB8888
		hostImageRB.pop((NMGL_IMAGE*)dstImage, 1);
#endif //  OUTPUT_IMAGE_RGB8888
	}
	else {
		while (hostImageRB.isEmpty()) {
			halSleep(2);
		}
		hostImageRB.incTail();
	}
	return 0;
};
