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
	v4nm64f srcV_64f[NMGL_SIZE];
#pragma data_section ".data_imu1"		
	v4nm64f srcC_64f = { 0,100,500,1000 };
#pragma data_section ".data_imu2"
	v4nm32f srcC = {0,100,500,1000};
	v4nm32f srcV[NMGL_SIZE];
	double dst[NMGL_SIZE];
#pragma data_section ".data_imu3"

extern "C" void dotC_gt0_64f(v4nm64f* normal, v4nm64f* C, double* dstValues, int size);

int main()
{
	
	int count=32;

	clock_t t0,t1;
	unsigned crc = 0;
	//nmppsRandUniform_64f((double*)srcV_64f, 4*NMGL_SIZE,-1000,1000);
	for(int i=0;i<NMGL_SIZE;i++){
		srcV_64f[i].vec[0] = 4*i+0-1000;
		srcV_64f[i].vec[1] = 4*i+1-1000;
		srcV_64f[i].vec[2] = 4*i+2-1000;
		srcV_64f[i].vec[3] = 4*i+3-1000;
	}

	for(int localSize=0;localSize<=NMGL_SIZE;localSize++){
		t0 = clock();
		dotC_gt0_64f(srcV_64f, &srcC_64f, dst, localSize);
		t1 = clock();
		nmppsCrcAcc_64f(dst,4, localSize, &crc);
	}

	for (int i = 0; i < 16; i++) {
		printf("%f\n", dst[i]);
	}

	printf("crc=0x%x\n", crc);
	return t1-t0;
}
