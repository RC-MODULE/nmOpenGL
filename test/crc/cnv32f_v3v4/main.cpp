#include "nmpp.h"
#include "time.h"
#include <stdio.h>

#define NMGL_SIZE 1024
#define MAX_STEP 4

#pragma data_section ".data_imu1"
	float srcVec[3*MAX_STEP*NMGL_SIZE];
#pragma data_section ".data_imu2"
	float dstVec[4*NMGL_SIZE+2];
	
	
extern "C" void cnv32f_v3v4(nm32f* srcVec, nm32f* dstVec, float value4, int countVec);

int main()
{
	for(int i=0;i<3*MAX_STEP*NMGL_SIZE;i++){
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
		cnv32f_v3v4(srcVec, dstVec, 1, size);
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
	return t1-t0;
}
