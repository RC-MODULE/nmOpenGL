#include "nmpp.h"
#include "stdio.h"
#include "time.h"

#define WIDTH_PTRN 32
#define HEIGHT_PTRN 32

#ifdef __GNUC__
#define SECTION(sec) __attribute__((section(sec)))
#else
#define SECTION(sec)
#endif // __GNUC__

///////////////////////////////////////////////////////////////////////////////////////
#define DST_SIZE (COUNT * WIDTH_PTRN * HEIGHT_PTRN)
#define COUNT 32

typedef int Pattern[WIDTH_PTRN * HEIGHT_PTRN / 16];

struct Rectangle{
	int x;
	int y;
	int width;
	int height;	
};


SECTION(".data_imu0")	Pattern patterns[COUNT];
SECTION(".data_imu0")	nm2s* pPatterns[COUNT];
SECTION(".data_imu1")	Rectangle windows[COUNT];
SECTION(".data_imu1")	int offsetsX[COUNT];
SECTION(".data_imu1")	int widths[COUNT];
SECTION(".data_imu1")	int heights[COUNT];
SECTION(".data_imu1")   int values[COUNT];

SECTION(".data_imu2") 	nm32s dst[DST_SIZE];



extern "C"{
	void mMulCVxN_2s32s(Pattern* ppSrcTreangle_2s, Rectangle* window, int* valueC, nm32s* pDstTreangle_32s, int count);
	void mMulCVxN_2s32sExt(nm2s** ppSrcTreangle_2s, int* offsets, int* widths, int* heights, nm32s* pDstTreangle, int* valueC,  int count);
}

int main()
{		
	unsigned int crc1 = 0;
	unsigned int crc2 = 0;
	clock_t t0, t1;
	nmppsRandUniform_32s((nm32s*)patterns, sizeof32(Pattern) * COUNT);
	for(int w=2; w <=32; w+=2){
		
		nmppsSet_32s(dst,0xCDCDCDCD,DST_SIZE);
		for(int i=0;i<COUNT;i++){
			pPatterns[i] = (nm2s*)(patterns + i);
			offsetsX[i] = 0;
			windows[i].x = 0;
			windows[i].y = 0;
			widths[i] = w;
			windows[i].width = w;
			heights[i] = 32;
			windows[i].height = 32;
			values[i] = i + 1;
		}
		t0 = clock();
		mMulCVxN_2s32sExt(pPatterns,
					offsetsX,
					widths,
					heights,
					dst,
					values, 
					COUNT);
		t1 = clock();
		nmppsCrcAcc_32s(dst, DST_SIZE, &crc1);
		printf("width=%d\n", w);
		printf("ext - time=%d, crc=0x%x\n", (int)(t1-t0), crc1);
		
		nmppsSet_32s(dst,0xCDCDCDCD,DST_SIZE);
		t0 = clock();
		mMulCVxN_2s32s(patterns, windows, values, dst, COUNT);
		t1 = clock();
		nmppsCrcAcc_32s(dst, DST_SIZE, &crc2);
		printf("new - time=%d, crc=0x%x\n\n", (int)(t1-t0), crc2);
	}
	
	return 0;
}
