#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"
#include "demo3d_common.h"

#define SIZE 4096
SECTION(".data_imu2") int src[SIZE];
	
SECTION(".data_imu3") int dst[SIZE+2];

int main()
{
	int src = 0xc010;
	int dst = 0x10800;
	clock_t t0,t1;
	
	unsigned int crc = 0;
	nmppsRandUniform_32s((nm32s*)src,SIZE);
	nmppsSet_32s((nm32s*)dst, 0xCDCDCDCD, SIZE + 2);
	for(int size =2;size<=SIZE;size+=2)
	{
		nmppsCmpLtC_32s31b((nm32s31b*)src, 128, (nm32s*)dst, 292);	//maxX<=0
		nmppsCrcAcc_32s((nm32s*)dst, size+2, &crc);
	}
	return crc>>2;
}
