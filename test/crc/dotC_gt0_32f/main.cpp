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

#define NMGL_SIZE 1024

#pragma data_section ".data_imu0"
	v4nm32f srcV[NMGL_SIZE];
#pragma data_section ".data_imu1"		
	v4nm32f srcC = {0,100,500,1000};
#pragma data_section ".data_imu2"
	nm32f dst[2 * NMGL_SIZE];
	nm32f dst2[2 * NMGL_SIZE];
#pragma data_section ".data_imu3"

extern "C" void dotC_gt0_32f(v4nm32f* normal, v4nm32f* C, nm32f* dstValues, int size);

int main()
{
	
	int count=32;

	clock_t t0,t1;
	unsigned crc=0;
	//nmppsRand_32f((nm32f*)srcV, 4*NMGL_SIZE,-1000,1000);
	for(int i=0;i<NMGL_SIZE;i++){
		srcV[i].vec[0] = 4*i+0-1000;
		srcV[i].vec[1] = 4*i+1-1000;
		srcV[i].vec[2] = 4*i+2-1000;
		srcV[i].vec[3] = 4*i+3-1000;
	}

	for(int localSize=0;localSize<=NMGL_SIZE;localSize++){
		t0 = clock();
		dotC_gt0_32f(srcV, &srcC, dst, localSize);
		t1 = clock();
		nmppsCrcAcc_32f(dst,4, localSize, &crc);
	}
	float ll[32];
	for(int i=0;i< 32; i++){
		ll[i] = 0;
	}
	float pow = 0;
	nmppsLn_32f(ll,dst2,32);

	for (int i = 0; i < 16; i++) {
		printf("%f\n", dst[i]);
		printf("ll=%f, 0x%x\n", dst2[i],dst2[i]);
	}

	printf("crc=0x%x\n", crc);

	
	return t1-t0;
}
