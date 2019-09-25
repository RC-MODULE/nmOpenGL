#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "demo3d_nm0.h"
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
	nm32f srcV[NMGL_SIZE];
#pragma data_section ".data_imu1"	
#pragma data_section ".data_imu2"
	nm32f dst[NMGL_SIZE+2];
#pragma data_section ".data_imu3"


int main()
{
	
	int count=32;

	clock_t t0,t1;
	unsigned crc=0;
	//nmppsRand_32f((nm32f*)srcV, 4*NMGL_SIZE,-1000,1000);
	for(int i=0;i<NMGL_SIZE;i++){
		srcV[i] = i;
	}
	for(int i=0;i<NMGL_SIZE+2;i++){
		dst[i] = 0;
	}
	
	for (int i = 0; i < 16; i++) {
		printf("%f\n", srcV[i]);
	}
	printf("\n");

	for(int localSize=0;localSize<=NMGL_SIZE;localSize+=2){
		t0 = clock();
		clamp_32f(srcV, 4, 12, dst, localSize);
		t1 = clock();
		nmppsCrcAcc_32f(dst,4, localSize+2, &crc);
	}

	for (int i = 0; i < 16; i++) {
		printf("%f\n", dst[i]);
	}

	printf("crc=0x%x\n", crc);

	
	return t1-t0;
}
