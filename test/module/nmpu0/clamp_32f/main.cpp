#include "nmpp.h"
#include "demo3d_nm0.h"
#include "time.h"

#define SIZE 1024


SECTION(".data_imu0")	nm32f srcV[SIZE];
SECTION(".data_imu1")	nm32f dst[SIZE + 2];


int main()
{
	

	clock_t t0,t1;
	unsigned crc=0;
	nmppsRand_32f((nm32f*)srcV, SIZE,-1000,1000);
	
	/*for(int i=0;i<SIZE;i++){
		srcV[i] = i;
	}*/
	
	for(int i = 0; i < SIZE + 2; i++){
		dst[i] = 0;
	}

	for(int localSize = 0; localSize <= SIZE; localSize+=2){
		t0 = clock();
		clamp_32f(srcV, -500, 500, dst, localSize);
		t1 = clock();
		nmppsCrcAcc_32f(dst,4, localSize+2, &crc);
	}

	printf("crc=0x%x\n", crc);	
	return t1-t0;
}
