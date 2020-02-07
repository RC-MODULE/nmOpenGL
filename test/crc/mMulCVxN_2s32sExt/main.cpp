#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"
#include "demo3d_common.h"

#define SIZE 2048
#pragma data_section ".data_imu0"

SECTION(".data_imu1") long long int src[SIZE/32];
		
SECTION(".data_imu2") nm64s aline;
SECTION(".data_imu2") int valuesC[32];
SECTION(".data_imu2") int offsets[32];
SECTION(".data_imu2") int widths[32];
SECTION(".data_imu2") int heights[32];
SECTION(".data_imu2") long long int* ppSrc[32];
	
SECTION(".data_imu3") int dst[32768];

extern "C" void mMulCVxN_2s32sExt(nm2s** ppSrcTreangle_2s, int* offsets, int* widths, int* heights, nm32s* pDstTreangle, int* valueC,  int count);

int main()
{		
	unsigned int crc = 0;
	clock_t t0, t1;
	for(int i = 0; i < 32768; i++){
		dst[i] = 0xCDCDCDCD;
	}
	nmppsRandUniform_32s((nm32s*)src,SIZE/32*2);
	nmppsAndC_32u((nm32u*)src, 0x55555555, (nm32u*)src,SIZE/32*2);
	
	for (int i = 0; i < 32; i++) {
		printf("dst[%d]=%d\n", i, dst[i]);
	}
	printf("\n");

//	for(int count=0;count<=32;count++){
//		for(int o = 0; o < 32; o++){
//			for(int h = 1; h < 33; h++){
//				for(int w=2;w<=(32 - o);w+=2){
					for(int i =0;i<32;i++){
						valuesC[i] = -1;
						offsets[i] = 0;
						widths[i] = 32;
						heights[i] = 32;
						ppSrc[i] = src;
					}
				
					t0 = clock();
					mMulCVxN_2s32sExt((nm2s**)ppSrc,offsets,widths,heights,(nm32s*)dst, valuesC, 1);
					t1 = clock();
					nmppsCrcAcc_32u((nm32u*)dst, SIZE,&crc);
//				}
//			}
//		}
//	}

	for (int i = 0; i < 32; i++) {
		printf("dst[%d]=%d\n", i, dst[i]);
	}
	printf("\n");

	printf("crc=0x%x\n",crc);
	return t1-t0;
}
