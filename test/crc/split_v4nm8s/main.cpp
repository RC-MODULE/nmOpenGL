#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"

#define SIZE 1024

SECTION(".data_imu1") v4nm8s srcVec4[SIZE];
SECTION(".data_imu2") int dstX[SIZE];
SECTION(".data_imu3") int dstY[SIZE];
SECTION(".data_imu4") int dstZ[SIZE];
SECTION(".data_imu5") int dstW[SIZE];
	
	
extern "C" void decimate_v4nm8s(v4nm8s* srcVec4, int startPos, nm8s* dstVec1, int size);

int main()
{

	nmppsRandUniform_32s((nm32s*)srcVec4, SIZE);
	nmppsSet_32s(dstX,0xCDCDCDCD, SIZE);
	clock_t t0,t1;
	unsigned crc =0;
	for(int size=8;size<= SIZE;size+=8){
		t0=clock();
		decimate_v4nm8s(srcVec4, 0, (nm8s*)dstX, 8);
		t1=clock();
		nmppsCrcAcc_32s(dstX, size/4 + 2,&crc);
	}
	for (int i = 0; i < 4; i++) {
		printf("0x%x\n",dstX[i]);
	}
	printf("\n");
	printf("0x%x\n",crc);
	return crc;
}
