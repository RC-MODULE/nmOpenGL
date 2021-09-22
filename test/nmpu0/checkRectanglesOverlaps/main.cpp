#include "nmpp.h"
#include "demo3d_nm0.h"
#include "time.h"

#define SIZE 1024


SECTION(".data_imu0")	v2nm32f minXY[SIZE];
SECTION(".data_imu1")	v2nm32f maxXY[SIZE];
SECTION(".data_imu2")	int mask[SIZE/32 + 2];

extern "C" void checkRectanglesOverlaps(v2nm32f* minXY, v2nm32f* maxXY, v2nm32f* upperRightLimit, v2nm32f* lowerLeftLimit, nm1* mask, int size);
extern "C" void checkRectanglesOverlapsOld(v2nm32f* minXY, v2nm32f* maxXY, v2nm32f* upperRightLimit, v2nm32f* lowerLeftLimit, nm1* mask, int size);

int main()
{
	

	clock_t t0,t1;
	unsigned crc=0;
	nmppsRand_32f((nm32f*)minXY, 2 * SIZE, 0, 3);
	nmppsRand_32f((nm32f*)maxXY, 2 * SIZE, 0, 3);

	v2nm32f upperRightLimit = {2,2};
	v2nm32f lowerLeftLimit = {1,1};

	//printf("upperRightLimit=%p, lowerLeftLimit=%p\n", &upperRightLimit, &lowerLeftLimit);
	//printf("upperRightLimit={%f,%f}, lowerLeftLimit={%f,%f}\n", upperRightLimit.v0, upperRightLimit.v1, lowerLeftLimit.v0, lowerLeftLimit.v1);

	//for(int i = 32; i < 64; i++){
	//	printf("%2d MinXY={%6.3f,%6.3f}, MaxXY={%6.3f,%6.3f}\n", i, minXY[i].v0, minXY[i].v1, maxXY[i].v0, maxXY[i].v1);
	//}
	for(int size=1; size< SIZE;size++){
		for(int i=0; i< SIZE/32 + 2; i++){
			mask[i] = 0xCDCDCDCD;
		}
		t0 = clock();
		checkRectanglesOverlaps(minXY, maxXY, &upperRightLimit, &lowerLeftLimit, mask, size);
		t1 = clock();
		nmppsCrcAcc_32s(mask, SIZE/32 + 2, &crc);
	}

	//printf("mask=0x%x\n", mask[1]);

	printf("crc=0x%x\n", crc);	
	return t1-t0;
}
