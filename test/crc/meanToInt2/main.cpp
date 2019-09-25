#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include <demo3d_nm0.h>

#define NMGL_SIZE 1024

#pragma data_section ".data_imu1"
	float src1[NMGL_SIZE];
#pragma data_section ".data_imu2"
	float src2[NMGL_SIZE];
#pragma data_section ".data_imu5"
	int result[NMGL_SIZE+2];
#pragma data_section ".data_imu6"
	
	

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
		result[i] = 0;
	}
	for(int size=2;size<= NMGL_SIZE;size+=2){
		t0=clock();
		meanToInt2(src1, src2, result, size);
		t1=clock();
		nmppsCrcAcc_32s(result, size + 2, &crc);
	}
	
	for(int i=0;i<8;i++){
		printf("result[%d]=%d\n", i, result[i]);
	}
	printf("0x%x\n",crc);
	return t1-t0;
}
