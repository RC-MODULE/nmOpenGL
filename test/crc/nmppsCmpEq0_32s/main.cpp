#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"

#pragma data_section ".data_imu0"

#pragma data_section ".data_imu1"
	float srcMatrix4x4[16]={0.0f,1.0f,2.0f,3.0f,
							4.0f,5.0f,6.0f,7.0f,
							8.0f,9.0f,10.0f,11.0f,
							12.0f,13.0f,14.0f,15.0f};
		
#pragma data_section ".data_imu2"
	float src[4096];
	
#pragma data_section ".data_imu3"
	float dst[4096];

#define SIZE_ONE 11*32

extern "C" void nmppsCmpEq0_32s(nm32s* pSrcVec, nm32s* pDstVec, int nSize);

int main()
{

	clock_t t0,t1;

	unsigned int crc = 0;
	nmppsRand_32f(src, 4096, 0.1f, 1000.0f);

	return crc;
}
