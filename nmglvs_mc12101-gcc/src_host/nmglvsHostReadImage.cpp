//------------------------------------------------------------------------
//  Demostration for MAX 2017 
//
// 
//
//  Copyright (c) 2017 RC Module Inc.
//------------------------------------------------------------------------

#include "VShell.h"
#include "demo3d_common.h"


extern ImageConnector hostImageRB;

//#define FAST

int nmglvsHostReadImage(int* dstImage)
{
	S_VS_MouseStatus mouseStatus;
	VS_GetMouseStatus(&mouseStatus);
#if defined(PROFILER0) || defined(PROFILER1) || defined (FAST)
	if (mouseStatus.nKey == VS_MOUSE_LBUTTON) {
#else
	if (mouseStatus.nKey != VS_MOUSE_LBUTTON) {
#endif
		hostImageRB.pop((ImageRGB8888*)dstImage, 1);
	}
	else {
		while (hostImageRB.isEmpty()) {
			halSleep(2);
		}
		hostImageRB.incTail();
	}
	return 0;
};
