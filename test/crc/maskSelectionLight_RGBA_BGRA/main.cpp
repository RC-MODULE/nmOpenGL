#include "nmpp.h"
#include "time.h"
#include <stdio.h>

#define NMGL_SIZE 1024
#define MAX_STEP 4

#pragma data_section ".data_imu1"
	v4nm32s srcVec[NMGL_SIZE];
#pragma data_section ".data_imu2"
	v4nm32s dstVec[NMGL_SIZE+2];
#pragma data_section ".data_imu2"
	int mask[NMGL_SIZE/32];
	
extern "C" int maskSelectionLight_RGBA_BGRA(v4nm32s* srcLight, nm1* mask, v4nm32s* dstLight, int size);

int main()
{
	int* src = (int*) srcVec;
	int* dst = (int*) dstVec;
	for(int i=0;i<NMGL_SIZE;i++){
		src[4 * i + 0] = i;
		src[4 * i + 1] = i + NMGL_SIZE;
		src[4 * i + 2] = i + 2 * NMGL_SIZE;
		src[4 * i + 3] = i + 3 * NMGL_SIZE;
	}
	for(int i=0;i<16;i++){
		printf("%d\n", src[i]);
	}
	printf("\n");
	for(int i=0;i<NMGL_SIZE+2;i++){
		dst[4 * i + 0] = 0;
		dst[4 * i + 1] = 0;
		dst[4 * i + 2] = 0;
		dst[4 * i + 3] = 0;
	}
	for(int i=0;i<NMGL_SIZE/32;i++){
		mask[i] = 0x0f0F0F0F;
	}
	clock_t t0,t1;
	unsigned crc =0;
	int f = 0;
	for(int size=1;size<= NMGL_SIZE;size++){
		//int size = 16;
		t0=clock();
		f = maskSelectionLight_RGBA_BGRA(srcVec, (nm1*)mask, dstVec, size);
		t1=clock();
		nmppsCrcAcc_32s((int*)dstVec, 4*size + 2, &crc);
		nmppsCrcAcc_32s(&f, 1, &crc);
	}
	
	printf("%d\n", f);
	for(int i=0;i<16;i++){
		printf("%d\n", dst[i]);
	}
	printf("crc = 0x%x\n", crc);
	return t1-t0;
}
