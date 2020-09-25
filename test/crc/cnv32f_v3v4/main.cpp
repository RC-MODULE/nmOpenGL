#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"

#define SIZE 1024

#pragma data_section ".data_imu1"
	float srcVec[3*SIZE];
#pragma data_section ".data_imu2"
	float dstVec[4*SIZE+2];
	
	
extern "C" void cnv32f_v3v4(nm32f* srcVec, nm32f* dstVec, float value4, int countVec);

int main()
{
	for(int i=0;i<3 * SIZE;i++){
		srcVec[i] = i;
	}
	for(int i=0;i<4 * SIZE+2;i++){
		dstVec[i] = 0;
	}
	for(int i=0;i<8;i++){
		printf("srcVec[%d]=%.1f ", i, srcVec[3*i+0]);
		printf("srcVec[%d]=%.1f ", i, srcVec[3*i+1]);
		printf("srcVec[%d]=%.1f\n", i, srcVec[3*i+2]);		
	}
	clock_t t0,t1;
	unsigned crc =0;
	for(int size=0;size<= SIZE;size++){
		t0=clock();
		cnv32f_v3v4(srcVec, dstVec, 1, size);
		t1=clock();
		nmppsCrcAcc_32f(dstVec, 2, 4*size+2,&crc);
	}
	printf("\n");
	//cnv32f_v3v4(srcVec, 1, dstVec, 1);
	for(int i=0;i<8;i++){
		printf("dstVec[%d]=%.1f ", i, dstVec[4*i+0]);
		printf("dstVec[%d]=%.1f ", i, dstVec[4*i+1]);
		printf("dstVec[%d]=%.1f ", i, dstVec[4*i+2]);
		printf("dstVec[%d]=%.1f\n", i, dstVec[4*i+3]);
	}
	printf("crc=0x%x\n", crc);
	return t1-t0;
}
