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

#define SIZE 1024


SECTION(".data_imu0")	int src1[SIZE];
SECTION(".data_imu1")	int src2[SIZE];
SECTION(".data_imu0")	int src3[SIZE];
SECTION(".data_imu1")	int src4[SIZE];
SECTION(".data_imu2")	int dst[4 * SIZE + 2];



extern "C"{
	void merge_v4nm32s(nm32s* src1, nm32s* src2, nm32s* src3, nm32s* src4, v4nm32s* dst, int size);
}

int main()
{		
	unsigned int crc1 = 0;
	unsigned int crc2 = 0;
	clock_t t0, t1;
	nmppsRandUniform_32s(src1, SIZE);
	nmppsRandUniform_32s(src2, SIZE);
	nmppsRandUniform_32s(src3, SIZE);
	nmppsRandUniform_32s(src4, SIZE);
	nmppsSet_32s(dst, 0xCDCDCDCD, 4 * SIZE + 2);
	for(int i=0;i<8;i++){
		printf("0x%8x,0x%8x,0x%8x,0x%8x\n", src1[i], src2[i], src3[i], src4[i]);
	}
	printf(".\n");
	merge_v4nm32s(src1,src2,src3,src4,(v4nm32s*)dst, 2);
	for(int i=0;i<8;i++){
		for(int j=0;j<4;j++){
			printf("0x%8x, ", dst[4 * i + j]);
		}
		printf(".\n");		
	}
	
	
	return 0;
}
