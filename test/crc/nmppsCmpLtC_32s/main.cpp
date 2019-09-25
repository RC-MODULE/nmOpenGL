#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"

#pragma data_section ".data_imu0"
#define NMGL_SIZE 4096
#pragma data_section ".data_imu1"
#pragma data_section ".data_imu2"
//	int src[NMGL_SIZE];
	
#pragma data_section ".data_imu3"
//	int dst[NMGL_SIZE+2];

int main()
{
	int src = 0xc010;
	int dst = 0x10800;
	clock_t t0,t1;
	
	unsigned int crc = 0;
	nmppsRandUniform_32s((nm32s*)src,NMGL_SIZE);
	nmppsSet_32s((nm32s*)dst, 0xCDCDCDCD, NMGL_SIZE + 2);
	for(int size =2;size<=NMGL_SIZE;size+=2)
	{
		nmppsCmpLtC_32s31b((nm32s31b*)src, 128, (nm32s*)dst, 292);	//maxX<=0
		nmppsCrcAcc_32s((nm32s*)dst, size+2, &crc);
	}
	return crc>>2;
}
