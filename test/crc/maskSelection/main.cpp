#include "nmpp.h"
#include "time.h"
#include <stdio.h>

#define		NMGL_SIZE 		1024

#pragma data_section ".data_imu1"
	float srcVertVec4[12*NMGL_SIZE+2];
#pragma data_section ".data_imu2"
	float srcNormVec4[12*NMGL_SIZE+2];
#pragma data_section ".data_imu3"
	nm32s mask[NMGL_SIZE/32+2];
#pragma data_section ".data_imu4"
	float dstVertVec4[12*NMGL_SIZE+2];
#pragma data_section ".data_imu5"
	float dstNormVec4[12*NMGL_SIZE+2];
	
	extern "C" int maskSelection_vertex_normal(float* srcVertVec4, float* srcNormVec4, nm1* mask, float* dstVertVec4, float* dstNormVec4,  int count);
	extern "C" int maskSelection_v2nm32f(float* srcVertVec4, nm1* mask, float* dstVertVec4,  int count);
	extern "C" int maskSelection_one(nm32f* src, int step, nm1* mask, nm32f* dst, int sizeMask);

int main()
{

	unsigned int crc = 0;
	for(int i=0;i<12*NMGL_SIZE;i++){
		srcVertVec4[i] = i;
		srcNormVec4[i] = i;
	}
	for (int i = 0; i<12 * NMGL_SIZE + 2; i++) {
		dstVertVec4[i] = 0;
		dstNormVec4[i] = 0;
	}
	for(int i=0;i<NMGL_SIZE/32+2;i++){
		mask[i] = 0x77777777;
	}
	int result = 0;
	for(int size=0;size<= NMGL_SIZE;size++){
		result = maskSelection_one(srcVertVec4, 1, mask, dstVertVec4,size);
		nmppsCrcAcc_32f(dstVertVec4,4, 12 * size+2,&crc);
	}
	/*for(int i=0;i<32;i++){
		printf("dstVertVec4[%3d]=",i);
		for(int j=0;j<4;j++)
			printf("%f, ",dstVertVec4[4*i+j]);
		printf("\n");
	}*/
/*	printf("\n");
	for(int i=0;i<32;i++){
		printf("dstNormVec4[%3d]=",i);
		for(int j=0;j<4;j++)
			printf("%f, ",dstNormVec4[4*i+j]);
		printf("\n");
	}*/
	printf("crc=0x%x\n", crc);
	printf("result=%d\n", result);
	return result;
}
