//------------------------------------------------------------------------
//  Demostration for MAX 2017 
//
//  Author: Ivan Zhilenkov
//
//  Copyright (c) 2017 RC Module Inc.
//------------------------------------------------------------------------

#include "hal.h"
#include "hal_host.h"

using namespace std;
extern int synchro_nm;

int nmglvsExit(unsigned* result)
{
	int exit = 1;
	halWriteMemBlock(&exit, synchro_nm + 4, 1, 0);
	halGetResult(result, 0);
	halGetResult(result+1, 1);
	return 0;
};
