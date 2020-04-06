#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"
#include "demo3d_common.h"

		
SECTION(".data_shmem0") int src[4096];
	
SECTION(".data_imu3") int dst[4096+2];

#define SIZE_ONE 11*32

extern "C" void selectPaintSide(nm32s* crossProd,int X, int Y, nm32s* pDst, int nSize);

int main()
{

	int X = 10;
	int Y = 15;
	clock_t t0,t1;
	unsigned crc =0;
	nmppsRandUniform_32s(src,4096);
	nmppsSet_32s(dst, 0xCDCDCDCD, 4096+2);
	for(int size=2;size<=4096;size+=2){
		t0 = clock();
		selectPaintSide(src,10, 15, dst, size);
		t1 = clock();
		nmppsCrcAcc_32s(dst, size+2, &crc);
	}	

	return crc;
}
