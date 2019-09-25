#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"

#define NMGL_SIZE 2048
#pragma data_section ".data_imu0"

#pragma data_section ".data_imu1"
	long long int src[NMGL_SIZE/32];
		
#pragma data_section ".data_imu2"
	nm64s aline;
	int valuesC[32];
	int offsets[32];
	int widths[32];
	int heights[32];
	long long int* ppSrc[32];
	
#pragma data_section ".data_imu3"
	int dst[32768];

#pragma data_section ".data_imu0"
extern "C" void mMulCVxN_2s32sExt(nm2s** ppSrcTreangle_2s, int* offsets, int* widths, int* heights, nm32s* pDstTreangle, int* valueC,  int count);

int main()
{		
	unsigned int crc = 0;
	clock_t t0, t1;
	for(int i=0;i<NMGL_SIZE;i++){
		dst[i] = 0xCDCDCDCD;
	}
	nmppsRandUniform_32s((nm32s*)src,NMGL_SIZE/32*2);
	nmppsAndC_32u((nm32u*)src, 0x55555555, (nm32u*)src,NMGL_SIZE/32*2);
	/*for(int i=0;i<32;i++){
		src[i] = 0x0000000000000000;
	}
	for(int i=22;i<23;i++){
		src[i] = 0x5555555555555550;
	}*/
	printf("srcVec\n");
	printf("src=0x%x\n",src);
	printf("dst=0x%x\n\n",dst);
	/*for(int i=0;i<32;i++){
		printf("0x%llx\n",src[i]);
	}*/
				printf("src=0x%x, dst =0x%x\n", src, dst);
	for(int count=0;count<=32;count++){
		//for(int h = 1;h<33;h++){
		//	for(int w=2;w<=(32 - o);w+=2){
			//for(int w=2;w<34;w+=2){
				for(int i =0;i<32;i++){
					valuesC[i] = 1;
					offsets[i] = 0;
					widths[i] = 32;
					heights[i] = 32;
					ppSrc[i] = src;
				}
				
				t0 = clock();
				mMulCVxN_2s32sExt((nm2s**)ppSrc,offsets,widths,heights,(nm32s*)dst, valuesC, count);
				t1 = clock();
				nmppsCrcAcc_32u((nm32u*)dst, NMGL_SIZE,&crc);
				printf("count=%2d, time=%5u, teoretic=%5d\n", count, t1-t0, count * 512);
				//printf("w=%2d, crc=0x%x\n", w,crc);
		//	}
			//printf("h=%d, crc=0x%x\n", h, crc);
		//}
	//printf("o=%d, crc=0x%x\n",o,crc);
	}
	printf("crc=0x%x\n",crc);
	/*for(int i=0;i<40;i++){
		printf("i=%d, 0x%x\n",i, dst[i]);
	}*/
	return t1-t0;
}
