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
	unsigned int time[2];
	unsigned synchroRB[2];
	int counter[2];
	int ok1 = halReadMemBlock(time, synchro_nm + 4, 2, 0);
	int ok2 = halReadMemBlock(counter, synchro_nm, 2, 0);
	int timeFrame = 0;
	printf("counter0=%d, timeFrame0=%10d\n", counter[0], time[0]);
	printf("counter1=%d, timeFrame1=%10d\n\n", counter[1], time[1]);
	imagesRB.tail++;
	return 0;
};
