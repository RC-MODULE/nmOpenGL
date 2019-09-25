#include "nmpp.h"
#include "time.h"
#include <stdio.h>

#define NMGL_SIZE 1024

#pragma data_section ".data_imu1"
	float src1[NMGL_SIZE+2];
#pragma data_section ".data_imu2"
	float src2[NMGL_SIZE+2];
#pragma data_section ".data_imu5"
	float min[NMGL_SIZE+2];
#pragma data_section ".data_imu6"
	float max[NMGL_SIZE+2];
	
	
extern "C" void findMinMax2(float* src1, float* src2, float* dstMin, float* dstMax, int size);

int main()
{
	
	clock_t t0,t1;
	unsigned crc =0;
	for(int i=0; i < NMGL_SIZE; i++){
		src1[i] = i;
		src2[i] = NMGL_SIZE - i;
	}
	//nmppsRand_32f(src1, NMGL_SIZE, 1,1000);
	//nmppsRand_32f(src2, NMGL_SIZE, 1,1000);
	for (int i = 0; i < NMGL_SIZE + 2; i++) {
		min[i] = 0;
		max[i] = 0;
	}
	for(int size=2;size<= NMGL_SIZE;size+=2){
		t0=clock();
		findMinMax2(src1, src2, min, max, size);
		t1=clock();
		nmppsCrcAcc_32f(min, 2, size + 2,&crc);
		nmppsCrcAcc_32f(max, 2, size + 2,&crc);
	}
/*	printf("src1[%d]=%.1f\n", 514, src1[514]);
	printf("src2[%d]=%.1f\n", 514, src2[514]);
	printf("src3[%d]=%.1f\n", 514, src3[514]);
	printf("min[%d]=%.1f\n", 514, min[514]);
	printf("max[%d]=%.1f\n", 514, max[514]);*/
	for(int i=0;i<8;i++){
		/*printf("src1[%d]=%.1f  ", i, src1[i]);
		printf("src2[%d]=%.1f  ", i, src2[i]);
		printf("src3[%d]=%.1f\n", i, src3[i]);*/
		printf("min[%d]=%.1f   ", i, min[i]);
		printf("max[%d]=%.1f\n", i, max[i]);
		//printf("\n");
	}
	printf("0x%x\n",crc);
	return t1-t0;
}
