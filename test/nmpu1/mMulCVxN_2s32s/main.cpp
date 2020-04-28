#include "nmpp.h"
#include "stdio.h"
#include "time.h"
#include "demo3d_nm1.h"

///////////////////////////////////////////////////////////////////////////////////////
#define DST_SIZE (COUNT * WIDTH_PTRN * HEIGHT_PTRN)
#define COUNT 32


SECTION(".data_imu0")	Pattern patterns[COUNT];
SECTION(".data_imu1")	Vector2 points[COUNT];
SECTION(".data_imu1")	Size sizes[COUNT];
SECTION(".data_imu1")   int values[COUNT];

SECTION(".data_imu2") 	nm32s dst[DST_SIZE];


int main()
{		
	unsigned int crc1 = 0;
	unsigned int crc2 = 0;
	clock_t t0, t1;
	nmppsRandUniform_32s((nm32s*)patterns, sizeof32(Pattern) * COUNT);
	nmppsAndC_32u((nm32u*)patterns, 0x55555555, (nm32u*)patterns, sizeof32(Pattern) * COUNT);
	for(int w=2; w <=32; w+=2){
		
		for(int i=0;i<COUNT;i++){
			points[i].x = 0;
			points[i].y = 0;
			sizes[i].width = w;
			sizes[i].height = 32;
			values[i] = i + 1;
		}
		
		nmppsSet_32s(dst,0xCDCDCDCD,DST_SIZE);
		t0 = clock();
		mMulCVxN_2s32s(patterns, points, sizes, values, dst, COUNT);
		t1 = clock();
		nmppsCrcAcc_32s(dst, DST_SIZE, &crc2);
		printf("new - time=%d, crc=0x%x\n\n", (int)(t1-t0), crc2);
	}
	
	return 0;
}
