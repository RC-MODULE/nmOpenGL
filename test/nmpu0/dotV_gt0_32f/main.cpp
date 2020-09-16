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

SECTION(".data_imu0") v4nm32f src1[SIZE];
SECTION(".data_imu1") v4nm32f src2[SIZE];
SECTION(".data_imu2") nm32f dst[2 * SIZE];

extern "C" void dotV_gt0_v4nm32f(v4nm32f* src1, v4nm32f* src2, v2nm32f* dstValues, int size);

int main()
{
	
	int count=32;

	clock_t t0,t1;
	unsigned crc=0;
	//nmppsRand_32f((nm32f*)srcV, 4*SIZE,-1000,1000);
	for(int i=0;i<SIZE;i++){
		src1[i].vec[0] = 1000 - 4*i+0;
		src1[i].vec[1] = 1000 - 4*i+1;
		src1[i].vec[2] = 1000 - 4*i+2;
		src1[i].vec[3] = 1000 - 4*i+3;
		
		src2[i].vec[0] = 1;
		src2[i].vec[1] = 1;
		src2[i].vec[2] = 1;
		src2[i].vec[3] = 1;
	}

	for (int i = 0; i < 16; i++) {
		printf("%f, %f, %f, %f\n", src1[i].vec[0], src1[i].vec[1], src1[i].vec[2], src1[i].vec[3]);
	}
	printf("\n");
	for (int i = 0; i < 16; i++) {
		printf("%f, %f, %f, %f\n", src2[i].vec[0], src2[i].vec[1], src2[i].vec[2], src2[i].vec[3]);
	}
	printf(".\n");
	for(int localSize=0;localSize<=SIZE;localSize++){
		t0 = clock();
		dotV_gt0_v4nm32f(src1, src2, (v2nm32f*)dst, localSize);
		t1 = clock();
		nmppsCrcAcc_32f(dst,4, localSize, &crc);
	}

	for (int i = 0; i < 16; i++) {
		printf("%f\n", dst[i]);
	}

	printf("crc=0x%x\n", crc);

	
	return t1-t0;
}
