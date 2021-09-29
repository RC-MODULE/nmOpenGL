#include "nmglvs_nmc1.h"
#include "demo3d_common.h"
#include "hal_target.h"
#include "link.h"
#include "nmprofiler.h"

SECTION(".data_imu0") NMGL_Framebuffer framebuffer;
int main()
{
	halSetProcessorNo(1);
	return 0x611D611D;
} 

