#include "hal.h"
#include "hal_target.h"
#include "demo3d_nm1.h"

#include "nmgl.h"
#include "nmglvs_nmc1.h"


SECTION(".data_imu0")	NMGL_Context_NM1 nmglContextNM1;

int main()
{ 
	nmglvsNm1Init(nmglContextNM1);
	while (nmglvsNm1Run(nmglContextNM1)) {
		nmglvsNm1Step(nmglContextNM1);
	}
	return 0x611D611D;
} 

