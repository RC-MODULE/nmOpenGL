#include "nmpp.h"
#include "stdio.h"
#include "time.h"
#include "demo3d_nm1.h"

///////////////////////////////////////////////////////////////////////////////////////
#define SIZE 16384
#define COUNT 32

SECTION(".data_imu0")	nm32u src1[SIZE];
SECTION(".data_imu1")	nm32u src2[SIZE];
		
SECTION(".data_imu2") nm32u* ppSrc1[COUNT];
SECTION(".data_imu2") nm32u* ppSrc2[COUNT];
SECTION(".data_imu2") nm32u* ppDst[COUNT];
SECTION(".data_imu2") int sizes[COUNT];
	
SECTION(".data_imu3")	nm32u dst[SIZE];



int main()
{		
	unsigned int crc = 0;
	clock_t t0, t1;
	nmppsRandUniform_32u(src1,SIZE);
	nmppsRandUniform_32u(src2,SIZE);
	nmppsSet_32u(dst,0xCDCDCDCD,SIZE);
	for(int i=0;i<COUNT;i++){
		ppSrc1[i] = src1 + 2 * i;
		ppSrc2[i] = src2 + 2 * i;
		ppDst[i] = dst;
		sizes[i] = 16;
	}
	mAndVxN_32u(ppSrc1, ppSrc2, ppDst, sizes, COUNT);
	nmppsCrcAcc_32u(dst,SIZE,&crc);
	return crc>>2;
}
