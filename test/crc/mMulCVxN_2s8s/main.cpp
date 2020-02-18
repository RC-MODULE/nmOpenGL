#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"
#include "demo3d_common.h"

///////////////////////////////////////////////////////////////////////////////////////
nm64s *L0;
nm64s *L1;
nm64s *G0;
nm64s *G1;
const int KB=1024/8;
const int SizeL0=30*KB;
const int SizeL1=30*KB;

const int SizeG0=30*KB;
const int SizeG1=30*KB;
extern "C" long tmp;

#pragma data_section ".data_imu0"

SECTION(".data_imu1") int src[8192/4];
		
SECTION(".data_imu2") nm64s aline;
SECTION(".data_imu2") int valuesC[32];
SECTION(".data_imu2") int offsets[32];
SECTION(".data_imu2") int widths[32];
SECTION(".data_imu2") int heights[32];
SECTION(".data_imu2") int* ppSrc[32];
	
SECTION(".data_imu3") int dst[2*8192 + 2];

#define SIZE_ONE 32*32

extern "C" void mMulCVxN_2s8s(nm2s** ppSrcTreangle_2s, int* offsets, int* widths, int* heights, nm8s* pDstTreangle_8s, int* valueC,  int count);

int main()
{		
	unsigned int crc = 0;
	clock_t t0, t1;
	nmppsRandUniform_32s(src,8192/4);
	nmppsRandUniform_32s(offsets,32);
	nmppsRandUniform_32s(widths,32);
	nmppsRandUniform_32s(heights,32);
	nmppsRandUniform_32s(valuesC,32);
	nmppsAndC_32u((nm32u*)offsets,0x7,(nm32u*)offsets,32);
	nmppsAndC_32u((nm32u*)src,0x55555555,(nm32u*)src,8192/4);
	nmppsAndC_32u((nm32u*)valuesC,0xFF,(nm32u*)valuesC,32);
	nmppsSet_32s(dst,0xCDCDCDCD,8192 + 2);
	for(int i=0;i<32;i++){
		printf("i=%d, valueC=%d\n",i,valuesC[i]);
	}
	printf("\n");
	nmppsAndC_32u((nm32u*)widths,0x1F,(nm32u*)widths,32);
	nmppsAndC_32u((nm32u*)heights,0x1F,(nm32u*)heights,32);
	for(int h=1;h<=32;h++){
		for(int w=0;w<=40;w+=8){
			for(int offset=0; offset<8;offset++){
				//int h = 32;
				//int w = 32;
				if(w==40 && offset==0){
					continue;
					//w=32;
				}
				for(int i=0;i<32;i++){
					offsets[i] = offset;
					widths[i] = w;
					heights[i] = h;	
					ppSrc[i] = src+32*16;
				}
				t0 = clock();
				mMulCVxN_2s8s((nm2s**)ppSrc,offsets,widths,heights,(nm8s*)dst, valuesC, 32);
				t1 = clock();
				nmppsCrcAcc_32u((nm32u*)dst, 8192,&crc);
				//printf("w=%2d, offset=%d, crc=0x%x\n",w,offset,crc);
				//printf("w=%2d, offset=%d, time=%u\n",w,offset,t1-t0);
			}
			printf("w=%2d, time=%u\n", w, t1-t0);
		}
		//printf("h=%2d, crc=0x%x\n", h, crc);
		printf("h=%2d, time=%u\n\n", h, t1-t0);		
	}
	//printf("time=%u\n",t1-t0);
	printf("crc=0x%x\n",crc);
	return crc>>2;
}
