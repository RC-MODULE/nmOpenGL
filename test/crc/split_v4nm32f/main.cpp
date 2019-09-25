#include "nmpp.h"
#include "time.h"
#include <stdio.h>

#define NMGL_SIZE 1024
#define MAX_STEP 4

#pragma data_section ".data_imu1"
	v4nm32f srcVec4[MAX_STEP*NMGL_SIZE];
#pragma data_section ".data_imu2"
	float dstX[NMGL_SIZE+2];
#pragma data_section ".data_imu3"
	float dstY[NMGL_SIZE+2];
#pragma data_section ".data_imu4"
	float dstZ[NMGL_SIZE+2];
#pragma data_section ".data_imu5"
	float dstW[NMGL_SIZE+2];
	
	
extern "C" void split_v4nm32f(v4nm32f* srcVec4, int step, float* dstX, float* dstY, float* dstZ, float* dstW, int countVec);

int main()
{
	for(int i=0;i<MAX_STEP*NMGL_SIZE/2;i++){
		srcVec4[i].vec[0] = 0;
		srcVec4[i].vec[1] = 1;
		srcVec4[i].vec[2] = 2;
		srcVec4[i].vec[3] = 3;
		/*srcVec4[2*i+0].vec[0] = 0;
		srcVec4[2*i+0].vec[1] = 1;
		srcVec4[2*i+0].vec[2] = 2;
		srcVec4[2*i+0].vec[3] = 3;
		srcVec4[2*i+1].vec[0] = 4;
		srcVec4[2*i+1].vec[1] = 5;
		srcVec4[2*i+1].vec[2] = 6;
		srcVec4[2*i+1].vec[3] = 8;*/
	}
	for(int i=0;i<NMGL_SIZE + 2;i++){
		dstX[i] = 0;
		dstY[i] = 0;
		dstZ[i] = 0;
		dstW[i] = 0;	
	}
	for(int i=0;i<8;i++){
		printf("srcVec4[%d]=%.1f ", i, srcVec4[i].vec[0]);
		printf("srcVec4[%d]=%.1f ", i, srcVec4[i].vec[1]);
		printf("srcVec4[%d]=%.1f ", i, srcVec4[i].vec[2]);
		printf("srcVec4[%d]=%.1f\n", i, srcVec4[i].vec[3]);
	}
	clock_t t0,t1;
	unsigned crc =0;
	for(int size=2;size<= NMGL_SIZE;size+=2){
		t0=clock();
		split_v4nm32f(srcVec4, 1, dstX, dstY, dstZ, dstW, size);
		t1=clock();
		nmppsCrcAcc_32f(dstX, 2, size + 2,&crc);
		nmppsCrcAcc_32f(dstY, 2, size + 2,&crc);
		nmppsCrcAcc_32f(dstZ, 2, size + 2,&crc);
		nmppsCrcAcc_32f(dstW, 2, size + 2,&crc);
	}
	printf("\n");
	/*for(int i=0;i<68;i++){
		printf("dstX[%d]=%.1f ", i, dstX[i]);
		printf("dstY[%d]=%.1f ", i, dstY[i]);
		printf("dstZ[%d]=%.1f ", i, dstZ[i]);
		printf("dstW[%d]=%.1f\n", i, dstW[i]);
	}*/
	printf("0x%x\n",crc);
	return crc;
}
