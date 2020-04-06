#include "nmpp.h"
#include "stdio.h"
#include "time.h"
//#include "demo3d_nm1.h"

#ifdef __GNUC__
#define SECTION(sec) __attribute__((section(sec)))
#else
#define SECTION(sec)
#endif // __GNUC__

#define ZBUFF_MAX 0x7FFFFFFF
#define ZBUFF_MAX_15s 0x7FFF
#define ZBUFF_INIT_VALUE ZBUFF_MAX

#define MAX_SIDE_POLYGON 32
#define HEIGHT_PTRN   MAX_SIDE_POLYGON
#define WIDTH_PTRN    MAX_SIDE_POLYGON
#define SMALL_SIZE 	  16
#define SIZE_BANK	0x8000
#define OFFSETS 	  WIDTH_PTRN
#define AMOUNT_ANGLES (2*WIDTH_PTRN + 2*HEIGHT_PTRN)
#define NPATTERNS 	  AMOUNT_ANGLES * OFFSETS * 2

///////////////////////////////////////////////////////////////////////////////////////
#define DST_SIZE (COUNT * WIDTH_PTRN * HEIGHT_PTRN)
#define COUNT 32

struct Rectangle {
	int x;
	int y;
	int width;
	int height;
};

typedef int Pattern[WIDTH_PTRN * HEIGHT_PTRN / 16];


SECTION(".data_imu0")	Pattern patterns[COUNT];
SECTION(".data_imu1")	Rectangle windows[COUNT];
SECTION(".data_imu1")   int values[COUNT];

SECTION(".data_imu2") 	nm32s dst[DST_SIZE];

extern "C" void mMulCVxN_2s16s(Pattern* ppSrcTreangle_2s, Rectangle* window, int* valueC, nm16s* pDstTreangle_32s, int count);
int main()
{		
	unsigned int crc = 0;
	clock_t t0, t1;
	nmppsRandUniform_32s((nm32s*)patterns, sizeof32(Pattern) * COUNT);
	nmppsAndC_32u((nm32u*)patterns, 0x55555555, (nm32u*)patterns, sizeof32(Pattern) * COUNT);
	for(int w=4; w <=32; w+=4){
		
		nmppsSet_32s(dst,0xCDCDCDCD,DST_SIZE);
		for(int i=0;i<COUNT;i++){
			windows[i].x = 0;
			windows[i].y = 0;
			windows[i].width = w;
			windows[i].height = 32;
			values[i] = i + 1;
		}
		nmppsSet_32s(dst,0xCDCDCDCD,DST_SIZE);
		t0 = clock();
		mMulCVxN_2s16s(patterns, windows, values, (nm16s*)dst, COUNT);
		t1 = clock();
		nmppsCrcAcc_32s(dst, DST_SIZE, &crc);
	}
	
	printf("crc=0x%x\n", crc);
	return 0;
}
