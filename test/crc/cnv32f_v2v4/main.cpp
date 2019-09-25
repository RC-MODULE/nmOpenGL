#include "nmpp.h"
#include "time.h"
#include <stdio.h>

#define NMGL_SIZE 1024
#define MAX_STEP 4

#pragma data_section ".data_imu1"
	float srcVec[2*MAX_STEP*NMGL_SIZE];
#pragma data_section ".data_imu2"
	float dstVec[4*NMGL_SIZE+2];
	
	
extern "C" void cnv32f_v2v4(v2nm32f* srcVec, int step, float value3, float value4, v4nm32f* dstVec, int countVec);

int main()
{
	for(int i=0;i<MAX_STEP*NMGL_SIZE;i++){
		srcVec[i] = i;
	}
	for(int i=0;i<4 * NMGL_SIZE+2;i++){
		dstVec[i] = 0;
	}
	for(int i=0;i<8;i++){
		printf("srcVec[%d]=%.1f ", i, srcVec[2*i+0]);
		printf("srcVec[%d]=%.1f\n", i, srcVec[2*i+1]);
	}
	clock_t t0,t1;
	unsigned crc =0;
	for(int size=2;size<= NMGL_SIZE;size+=2){
		t0=clock();
		cnv32f_v2v4((v2nm32f*)srcVec, 0, 10, 15, (v4nm32f*)dstVec, size);
		t1=clock();
		nmppsCrcAcc_32f(dstVec, 2, 4*size+2,&crc);
	}
	printf("\n");
	for(int i=0;i<8;i++){
		printf("dstVec[%d]=%.1f ", i, dstVec[4*i+0]);
		printf("dstVec[%d]=%.1f ", i, dstVec[4*i+1]);
		printf("dstVec[%d]=%.1f ", i, dstVec[4*i+2]);
		printf("dstVec[%d]=%.1f\n", i, dstVec[4*i+3]);
	}
	printf("crc=0x%x\n", crc);
	return crc;
}
