#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"

#define SIZE 1024
#define MAX_STEP 4

#pragma data_section ".data_imu1"
	float srcVec1[SIZE];
	float srcVec2[SIZE];
	float srcVec3[SIZE];
	float srcVec4[SIZE];
#pragma data_section ".data_imu2"
	float dstVec1[SIZE+2];
	float dstVec2[SIZE+2];
	float dstVec3[SIZE+2];
	float dstVec4[SIZE+2];
#pragma data_section ".data_imu2"
	int mask[SIZE/32];
	
float* srcPnt[4] = {srcVec1, srcVec2, srcVec3, srcVec4};
float* dstPnt[4] = {dstVec1, dstVec2, dstVec3, dstVec4};
	
extern "C" int copyArraysByMask(float** srcPointers, nm1* mask, float** dstPointers, int nArrays, int size);

int main()
{
	for(int i=0;i<SIZE;i++){
		srcVec1[i] = i;
		srcVec2[i] = i + SIZE;
		srcVec3[i] = i + 2 * SIZE;
		srcVec4[i] = i + 3 * SIZE;
	}
	for(int i=0;i<16;i++){
		printf("%f\n", srcVec1[i]);
	}
	printf("\n");
	for(int i=0;i<SIZE+2;i++){
		dstVec1[i] = 0;
		dstVec2[i] = 0;
		dstVec3[i] = 0;
		dstVec4[i] = 0;
	}
	for(int i=0;i<SIZE/32;i++){
		mask[i] = 0x0f0F0F0F;
	}
	clock_t t0,t1;
	unsigned crc =0;
	int f = 0;
	for(int size=1;size<= SIZE;size++){
		//int size = 16;
		t0=clock();
		f = copyArraysByMask(srcPnt, (nm1*)mask, dstPnt, 4, size);
		t1=clock();
		nmppsCrcAcc_32f(dstVec1, 2, size + 2, &crc);
		nmppsCrcAcc_32f(dstVec2, 2, size + 2, &crc);
		nmppsCrcAcc_32f(dstVec3, 2, size + 2, &crc);
		nmppsCrcAcc_32f(dstVec4, 2, size + 2, &crc);
		nmppsCrcAcc_32s(&f, 1, &crc);
	}
	
	printf("%d\n", f);
	for(int i=0;i<16;i++){
		printf("%f\n", dstVec1[i]);
	}
	printf("crc = 0x%x\n", crc);
	return t1-t0;
}
