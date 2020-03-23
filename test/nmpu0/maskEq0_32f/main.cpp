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

SECTION(".data_imu0") v4nm32f src1[SIZE];
SECTION(".data_imu1") v2nm32f src2[SIZE];
SECTION(".data_imu2") v4nm32f dst[2 * SIZE+2];

extern "C" void maskEq0(v4nm32f* srcVec, v2nm32f* srcMask, v4nm32f* dstVec, int size);

int main()
{
	
	int count=32;

	clock_t t0,t1;
	unsigned crc=0;
	//nmppsRand_32f((nm32f*)srcV, 4*SIZE,-1000,1000);
	for(int i=0;i<SIZE;i++){
		src1[i].vec[0] = 4*i+0;
		src1[i].vec[1] = 4*i+1;
		src1[i].vec[2] = 4*i+2;
		src1[i].vec[3] = 4*i+3;
	}
	for(int i=0;i<SIZE/2;i++){
		src2[2 * i + 0].v0 = 1;
		src2[2 * i + 0].v1 = 1;
		src2[2 * i + 1].v0 = 0;
		src2[2 * i + 1].v1 = 0;
	}
	for(int i=0;i<SIZE/2+2;i++){
		dst[i].vec[0] = 15;
		dst[i].vec[1] = 15;
		dst[i].vec[2] = 15;
		dst[i].vec[3] = 15;
	}



	for (int i = 0; i < 16; i++) {
		printf("%f, %f, %f, %f\n", src1[i].vec[0], src1[i].vec[1], src1[i].vec[2], src1[i].vec[3]);
	}
	printf("\n");
	for (int i = 0; i < 16; i++) {
		printf("%f, %f\n", src2[i].v0, src2[i].v1);
	}
	printf("\n");
	for(int localSize=0;localSize<=SIZE;localSize++){
		t0 = clock();
		maskEq0(src1, src2, dst, 1024);
		t1 = clock();
		nmppsCrcAcc_32f((float*)dst,4, 4*localSize + 2, &crc);
	}

	for (int i = 0; i < 16; i++) {
		printf("%f, %f, %f, %f\n", dst[i].vec[0], dst[i].vec[1], dst[i].vec[2], dst[i].vec[3]);
	}

	printf("crc=0x%x\n", crc);

	
	return t1-t0;
}
