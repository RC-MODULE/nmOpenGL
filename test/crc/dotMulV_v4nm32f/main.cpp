#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"

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

#pragma data_section ".data_imu0"
	v2nm32f src1[SIZE];
#pragma data_section ".data_imu1"		
	v4nm32f src2[SIZE];
#pragma data_section ".data_imu2"
	v4nm32f dst[SIZE + 1];
#pragma data_section ".data_imu3"

extern "C" void dotMulV_v4nm32f(v2nm32f* src1, v4nm32f* src2, v4nm32f* dstValues, int size);

int main()
{
	
	int count=32;

	clock_t t0,t1;
	unsigned crc=0;
	//nmppsRand_32f((nm32f*)srcV, 4*SIZE,-1000,1000);
	for(int i=0;i < SIZE; i++){
		src2[i].vec[0] = 4*i+0;// - 1000;
		src2[i].vec[1] = 4*i+1;// - 1000;
		src2[i].vec[2] = 4*i+2;// - 1000;
		src2[i].vec[3] = 4*i+3;// - 1000;
		
		src1[i].v0 = 1;
		src1[i].v1 = 1;
		
		dst[i].vec[0] = 0;
		dst[i].vec[1] = 0;
		dst[i].vec[2] = 0;
		dst[i].vec[3] = 0;
	}
	dst[SIZE].vec[0] = 0;
	dst[SIZE].vec[1] = 0;
	dst[SIZE].vec[2] = 0;
	dst[SIZE].vec[3] = 0;

	for (int i = 0; i < 16; i++) {
		printf("%f, %f \n", src1[i].v0, src1[i].v1);
	}
	printf("\n");
	for (int i = 0; i < 16; i++) {
		printf("%f, %f, %f, %f\n", src2[i].vec[0], src2[i].vec[1], src2[i].vec[2], src2[i].vec[3]);
	}
	printf("\n");
	for(int localSize=0;localSize<=SIZE;localSize++){
		t0 = clock();
		dotMulV_v4nm32f(src1, src2, dst, localSize);
		t1 = clock();
		nmppsCrcAcc_32f((float*)dst,4, 4 * localSize + 2, &crc);
	}

	for (int i = 492; i < 508; i++) {
		printf("%f  ", dst[i].vec[0]);
		printf("%f  ", dst[i].vec[1]);
		printf("%f  ", dst[i].vec[2]);
		printf("%f\n", dst[i].vec[3]);
	}

	printf("crc=0x%x\n", crc);

	
	return t1-t0;
}
