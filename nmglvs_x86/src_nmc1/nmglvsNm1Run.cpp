#include "hal_target.h"
#include "hal.h"
#include "hal.h"
#include "hal_target.h"
#include "nmpp.h"
#include "demo3d_nm1.h"
#include "stdio.h"
#include "ringbuffer.h"
#include "nmprofiler.h"
#include "cache.h"

#include "nmgl.h"
#include "nmgl_data1.h"


//#pragma code_section ".text_demo3d"

extern int exitNM1;

int nmglvsNm1Run()
{
	if(exitNM1 == 0){
		return 1;
	}
	else{
		return 0;
	}
} 

