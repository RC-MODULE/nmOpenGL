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
	nmppsCopy_32s(src, dstImage, WIDTH_IMAGE*HEIGHT_IMAGE);
	unsigned int time[2];
	unsigned synchroRB[2];
	int ok1 = halReadMemBlock(time, synchro_nm + 6, 2, 0);
	int ok2 = halReadMemBlock(synchroRB, synchro_nm + 10, 2, 0);
	int counter[2];
	halReadMemBlock(counter, synchro_nm, 2, 0);
	//printf("delayTime0=%u, delayTime1=%u\n", time[0], time[1]);

	imagesRB.tail++;
	return 0;
};
