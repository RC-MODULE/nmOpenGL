#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"

SECTION(".data_imu1")	float src1[SIZE];
SECTION(".data_imu2")	float src2[SIZE];
SECTION(".data_imu5")	int result[SIZE+2];	
	

int main()
{
	
	clock_t t0,t1;
	unsigned crc =0;
	for(int i=0; i < SIZE; i++){
		src1[i] = i;
		src2[i] = SIZE - i;
	}
	//nmppsRand_32f(src1, SIZE, 1,1000);
	//nmppsRand_32f(src2, SIZE, 1,1000);
	for (int i = 0; i < SIZE + 2; i++) {
		result[i] = 0;
	}
	for(int size=2;size<= SIZE;size+=2){
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
