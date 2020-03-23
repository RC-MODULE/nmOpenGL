#include "nmpp.h"
#include "stdio.h"
#include "time.h"
#include "demo3d_nm1.h"

///////////////////////////////////////////////////////////////////////////////////////
#define SIZE 16384
#define COUNT 32

SECTION(".data_imu1")	int src[SIZE];
		
SECTION(".data_imu2") int* ppSrc[COUNT];
SECTION(".data_imu2") int* ppDst[COUNT];
SECTION(".data_imu2") int sizes[COUNT];
	
SECTION(".data_imu3")	int dst[SIZE];



int main()
{		
	unsigned int crc = 0;
	clock_t t0, t1;
	nmppsRandUniform_32s(src,SIZE);
	nmppsSet_32s(dst,0xCDCDCDCD,SIZE);
	for(int i=0;i<COUNT;i++){
		ppSrc[i] = src;
		ppDst[i] = dst;
		sizes[i] = 16;
	}
	copyPacket_32s(ppSrc, ppDst, sizes, COUNT);
	nmppsCrcAcc_32s(dst,SIZE,&crc);
	return crc>>2;
}
