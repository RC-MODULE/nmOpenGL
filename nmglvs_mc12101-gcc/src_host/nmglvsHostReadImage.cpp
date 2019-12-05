//------------------------------------------------------------------------
//  Demostration for MAX 2017 
//
//  Author: Alexander Bolornikov
//
//  Copyright (c) 2017 RC Module Inc.
//------------------------------------------------------------------------

#include "VShell.h"
#include "hal.h"
#include "hal_host.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "nmpp.h"
#include "demo3d_host.h"
#include "demo3d_nm1.h"
#include "ringbuffer_host.h"
#include <thread>

extern HalRingBuffer imagesRB;
extern int synchro_nm;

int nmglvsHostReadImage(int* dstImage)
{
	while (halRingBufferIsEmpty(&imagesRB)) {
		halSleep(2);
	}
	int* src = (int*)halRingBufferTail(&imagesRB);
	nmppsCopy_32s(src, dstImage, imagesRB.size);
	imagesRB.tail++;
	return 0;
};
