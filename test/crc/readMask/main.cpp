#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"

#define		MAX_SIZE 		128
#define		LIMIT 			33



SECTION(".data_imu2") nm32s mask[MAX_SIZE/32+2];
SECTION(".data_imu3") int dstVec[MAX_SIZE+2];
	
extern "C" int readMask(nm1* mask, int* dstIndices, int size);

int main()
{
	clock_t t0, t1;
	int time = 0;
	unsigned int crc = 0;
	for (int i = 0; i < MAX_SIZE + 2; i++) {
		dstVec[i] = 0;
	}
	for(int i=0;i<MAX_SIZE/32+2;i++){
		mask[i] = 0xFFFF5554;
	}
	int result = 0;
	//for(int size=0;size<= MAX_SIZE;size++){
	int size = MAX_SIZE;
		printf("mask=0x%x\n", mask);
		printf("size=0x%x\n\n", size);
	
		nm1* maskTmp = (nm1*)mask;
		t0 = clock();
		result = readMask(maskTmp, dstVec, size);
		t1 = clock();
		time = MAX(t1 - t0, time);
	
		printf("result=%d\n", result);
		for(int i=0; i < result; i++){
			printf("dst[%d]=%d\n", i, dstVec[i]);
		}
		printf(".\n");
		
		nmppsCrcAcc_32s(dstVec, result + 2, &crc);
	//}
	printf("crc=0x%x\n", crc);
	return time;
}
