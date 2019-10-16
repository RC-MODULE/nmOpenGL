#include "nmpp.h"
#include "time.h"
#include <stdio.h>

#define		MAX_SIZE 		64
#define		LIMIT 			32

#pragma data_section ".shared0"

#pragma data_section ".data_imu1"
	float srcVec[MAX_SIZE];
#pragma data_section ".data_imu2"
	nm32s mask[MAX_SIZE/32+2];
#pragma data_section ".data_imu3"
	float dstVec[MAX_SIZE+2];
	
extern "C" int maskSelection(void** src, nm1** mask, void* dst, int* size, int maxSize);

int main()
{
	clock_t t0, t1;
	int time = 0;
	unsigned int crc = 0;
	for(int i=0;i < MAX_SIZE; i++){
		srcVec[i] = i;
	}
	for (int i = 0; i < MAX_SIZE + 2; i++) {
		dstVec[i] = 0;
	}
	for(int i=0;i<MAX_SIZE/32+2;i++){
		mask[i] = 0x77777777;
	}
	int result = 0;
	//for(int size=0;size<= MAX_SIZE;size++){
		int size = MAX_SIZE;
		
		printf("srcVec=0x%x\n", srcVec);
		printf("mask=0x%x\n", mask);
		printf("size=0x%x\n\n", size);
	
		int sizeTmp = size;
		float* srcTmp = srcVec;
		nm1* maskTmp = (nm1*)mask;
		while (sizeTmp > 0) {
			t0 = clock();
			result = maskSelection((void**)&srcTmp, &maskTmp, dstVec, &sizeTmp, LIMIT);
			t1 = clock();
			time = MAX(t1 - t0, time);
	
			printf("srcTmp=0x%x\n", srcTmp);
			printf("maskTmp=0x%x\n", maskTmp);
			printf("sizeTmp=0x%x\n", sizeTmp);
			printf("result=%d\n\n", result);
			for(int i=0; i< result; i++){
				printf("dst[%d]=%f\n", i, dstVec[i]);
			}
			
			nmppsCrcAcc_32f(dstVec, 4, size + 2, &crc);
		}
	//}
	printf("crc=0x%x\n", crc);
	return time;
}
