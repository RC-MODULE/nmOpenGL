#ifndef __DATA_NMC0__
#define __DATA_NMC0__
#include "nmgl.h"
#include "demo3d_nm0.h"
#include "ringbuffer.h"
#include <time.h>

SECTION(".data_shared")	HalRingBuffer* nmglPolygonsRB;

SECTION(".data_imu7")  NMGL_Context_NM0 cntxt;
#endif