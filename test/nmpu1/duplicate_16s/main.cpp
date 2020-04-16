#include "nmpp.h"
#include "stdio.h"
#include "time.h"
#include "demo3d_nm1.h"

///////////////////////////////////////////////////////////////////////////////////////
#define SIZE 16384

SECTION(".data_imu1")	int src[SIZE];
	
SECTION(".data_imu3")	int dst[SIZE + 2];



int main()
{		
	unsigned int crc = 0;
	clock_t t0, t1;
	nmppsRandUniform_32s(src,SIZE);
	nmppsSet_32s(dst,0xCDCDCDCD,SIZE + 2);
	
	for(int size = 0; size <= SIZE; size += 4 ){
		t0 = clock();
		duplicate_16s((nm16s*)src, dst, size);
		t1 = clock();
		nmppsCrcAcc_32s(dst, size + 2, &crc);
	}
	for(int i=0;i < 8;i++){
		printf("dst[%i]=0x%x\n", i, dst[i]);
	}
	printf("crc=0x%x\n", crc);
	return t1-t0;
}
