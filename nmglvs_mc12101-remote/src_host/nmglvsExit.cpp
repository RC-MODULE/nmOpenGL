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


int nmglvsExit(unsigned* result)
{
	halGetResult(result, 0);
	halGetResult(result + 1, 1);
	return 0;
};
