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
	float src[NMGL_SIZE];
#pragma data_section ".data_imu1"		
#pragma data_section ".data_imu2"
	float dst[NMGL_SIZE];
#pragma data_section ".data_imu3"

extern "C" void copyOddOffset(float* src, float* dst, int first, int size);

int main()
{
	clock_t t0,t1;
	for(int i=0; i< NMGL_SIZE; i++){
		src[i] = i;
	}
	
	for(int i=0; i< NMGL_SIZE; i++){
		dst[i] = 5;
	}
	
	t0 = clock();
	copyOddOffset(src, dst, 1, NMGL_SIZE);
	t1 = clock();
	for(int i = 0;i<32;i++){
		printf("%f\n", dst[i]);
	}
	
	return t1-t0;
}
