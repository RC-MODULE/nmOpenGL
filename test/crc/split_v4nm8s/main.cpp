#include "nmpp.h"
#include "time.h"
#include <stdio.h>

#define NMGL_SIZE 1024

#pragma data_section ".data_imu1"
	v4nm8s srcVec4[NMGL_SIZE];
#pragma data_section ".data_imu2"
	int dstX[NMGL_SIZE];
#pragma data_section ".data_imu3"
	int dstY[NMGL_SIZE];
#pragma data_section ".data_imu4"
	int dstZ[NMGL_SIZE];
#pragma data_section ".data_imu5"
	int dstW[NMGL_SIZE];
	
	
extern "C" void decimate_v4nm8s(v4nm8s* srcVec4, int startPos, nm8s* dstVec1, int size);

int main()
{

	nmppsRandUniform_32s((nm32s*)srcVec4, NMGL_SIZE);
	nmppsSet_32s(dstX,0xCDCDCDCD, NMGL_SIZE);
	clock_t t0,t1;
	unsigned crc =0;
	for(int size=8;size<= NMGL_SIZE;size+=8){
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
