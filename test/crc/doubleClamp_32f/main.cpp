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

#define SIZE 1024

SECTION(".data_imu1")	nm32f src1[SIZE];
SECTION(".data_imu2")	nm32f src2[SIZE];
SECTION(".data_imu3")	nm32f dst1[SIZE + 2];
SECTION(".data_imu4")	nm32f dst2[SIZE + 2];

extern "C" void doubleClamp_32f(float* src1, float* src2, float min, float max, float* dst1, float* dst2, int size);

int main()
{
	
	int count=32;

	clock_t t0,t1;
	unsigned crc=0;
	//nmppsRand_32f((nm32f*)srcV, 4*NMGL_SIZE,-1000,1000);
	for(int i = 0; i < SIZE; i++){
		src1[i] = i;
		src2[i] = i;
	}
	for(int i = 0; i < SIZE + 2; i++){
		dst1[i] = 0;
		dst2[i] = 0;
	}

	for (int i = 0; i < 16; i++) {
		printf("%f, %f\n", src1[i], src2[i]);
	}
	printf(".\n");
	for(int localSize = 0; localSize <= SIZE; localSize+=2){
		t0 = clock();
		doubleClamp_32f(src1, src2, 4, 8, dst1, dst2, localSize);
		t1 = clock();
		nmppsCrcAcc_32f(dst1, 4, localSize + 2, &crc);
		nmppsCrcAcc_32f(dst2, 4, localSize + 2, &crc);
	}

	for (int i = 0; i < 16; i++) {
		printf("%f %f\n", dst1[i], dst2[i]);
	}

	printf("crc=0x%x\n", crc);

	
	return t1-t0;
}
