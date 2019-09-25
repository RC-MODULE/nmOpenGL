#include "nmpp.h"
#include "time.h"
#include <stdio.h>

#define NMGL_SIZE 1024
#define MAX_STEP 4

#pragma data_section ".data_imu1"
	v2nm32f srcVec4[MAX_STEP*NMGL_SIZE];
#pragma data_section ".data_imu2"
	float dstX[NMGL_SIZE+2];
#pragma data_section ".data_imu3"
	float dstY[NMGL_SIZE+2];
	
	
extern "C" void split_v2nm32f(v2nm32f* srcVec4, int step, float* dstX, float* dstY, int countVec);

int main()
{
	for(int i=0;i<MAX_STEP*NMGL_SIZE/2;i++){
		srcVec4[i].v0 = 0;
		srcVec4[i].v1 = 1;
	}
	for(int i=0;i<NMGL_SIZE + 2;i++){
		dstX[i] = 0;
		dstY[i] = 0;	
	}
	for(int i=0;i<8;i++){
		printf("srcVec4[%d]=%.1f  ", i, srcVec4[i].v0);
		printf("srcVec4[%d]=%.1f\n", i, srcVec4[i].v1);
	}
	clock_t t0,t1;
	unsigned crc =0;
	for(int size=2;size<= NMGL_SIZE;size+=2){
		t0=clock();
		split_v2nm32f(srcVec4, 1, dstX, dstY, size);
		t1=clock();
		nmppsCrcAcc_32f(dstX, 2, size + 2,&crc);
		nmppsCrcAcc_32f(dstY, 2, size + 2,&crc);
	}
	printf("\n");
	for(int i=0;i<4;i++){
		printf("dstX[%d]=%.1f  ", i, dstX[i]);
		printf("dstY[%d]=%.1f\n", i, dstY[i]);
	}
	printf("0x%x\n",crc);
	return crc;
}
