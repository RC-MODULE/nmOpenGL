#include "nmpp.h"
#include "stdio.h"
#include "time.h"
#include "demo3d_nm1.h"
#include "cache.h"

///////////////////////////////////////////////////////////////////////////////////////
#define DST_SIZE (COUNT * WIDTH_PTRN * HEIGHT_PTRN)
#define COUNT 32


SECTION(".data_imu0")	Pattern patterns[COUNT];
SECTION(".data_imu1")	Vector2 points[COUNT];
SECTION(".data_imu1")	Size sizes[COUNT];
SECTION(".data_imu1")   int values[COUNT];
SECTION(".data_imu1")	nm32s src[32];

SECTION(".data_imu2") 	nm32s dst[DST_SIZE];

extern "C" void mMulCVxN_2s32s(Pattern* patterns, Vector2* innerPoint, Size* sizes, int* valueC, nm32s* pDstTreangle, int count);
extern "C" void mMulCVxN_2s_RGB8888(Pattern* patterns, Vector2* innerPoint, Size* sizes, int* valueC, nm32s* pDstTreangle_32s, int count);
extern "C" void mMulCVxN_2s32s_old(Pattern* patterns, Vector2* innerPoint, Size* sizes, int* valueC, nm32s* pDstTreangle, int count);
extern "C" void mMulCVxN_2s_RGB8888_old(Pattern* patterns, Vector2* innerPoint, Size* sizes, int* valueC, nm32s* pDstTreangle_32s, int count);

extern "C" void rep_data_perf(void *src, void *dst, int repN, int count);

int main()
{		
	unsigned int crc1 = 0;
	unsigned int crc2 = 0;
	clock_t t0, t1;
	nmppsRandUniform_32s((nm32s*)patterns, sizeof32(Pattern) * COUNT);
	nmppsAndC_32u((nm32u*)patterns, 0x55555555, (nm32u*)patterns, sizeof32(Pattern) * COUNT);

	halInstrCacheEnable();

	int count = 16;
	for(int n = 0; n <= 32; n++){
		t0 = clock();
		for(int i=0; i < 10; i++){
			rep_data_perf(src, dst, n, count);
		}
		t1 = clock();
		int time = t1 - t0;
		printf("repN=%d, time=%d\n", n, time/(count * 10));
	}

	return 0;

	for(int w=2; w <=32; w+=2){
		
		for(int i=0;i<COUNT;i++){
			points[i].x = 0;
			points[i].y = 0;
			sizes[i].width = 32;
			sizes[i].height = w;
			values[i] = i + 1;
		}
		printf("height=%d\n",w);
		
		//nmppsSet_32s(dst,0xCDCDCDCD,DST_SIZE);
		nmppsSet_32s(dst,0,DST_SIZE);
		t0 = clock();
		mMulCVxN_2s_RGB8888(patterns, points, sizes, values, dst, COUNT);
		t1 = clock();
		nmppsCrcAcc_32s(dst, DST_SIZE, &crc2);
		printf("mask - time=%5d, crc=0x%x\n", (int)(t1-t0)/32, crc2);

		nmppsSet_32s(dst,0,DST_SIZE);
		t0 = clock();
		mMulCVxN_2s_RGB8888_old(patterns, points, sizes, values, dst, COUNT);
		t1 = clock();
		nmppsCrcAcc_32s(dst, DST_SIZE, &crc1);
		printf("old - time=%5d, crc=0x%x\n\n", (int)(t1-t0)/32, crc1);
		/*for(int y = 0; y < sizes[0].height; y++){
			for(int x = 0; x < sizes[0].width; x++){
				printf("%d  ", dst[y * sizes[0].width + x]);
			}
			printf(".\n");
		}*/
	}
	
	return 0;
}
