//------------------------------------------------------------------------
//  Demostration for MAX 2017 
//
//  Author: Ivan Zhilenkov
//
//  Copyright (c) 2017 RC Module Inc.
//------------------------------------------------------------------------

#include "hal.h"
#include "hal_host.h"
#include "ringbuffer_host.h"

using namespace std;
extern HalHostRingBuffer hostImageRB; 
extern HalRingBuffer imagesRB;

int nmglvsExit(unsigned* result)
{
	int exit = 1;
	while (!halHostRingBufferIsEmpty(&hostImageRB)) {
		halHostRingBufferPop(&hostImageRB, halRingBufferHead(&imagesRB), 1);
	}
	halGetResult(result, 0);
	halGetResult(result + 1, 1);
	return 0;
};
