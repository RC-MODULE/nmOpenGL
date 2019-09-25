#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"
#include "demo3d_nm1.h"

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

#define NMGL_SIZE 16384/8

#pragma data_section ".data_imu0"
	float src1[NMGL_SIZE];
#pragma data_section ".data_imu1"
	float src2[NMGL_SIZE];		
#pragma data_section ".data_imu2"
	float src3[NMGL_SIZE];	
#pragma data_section ".data_imu3"
	float src4[NMGL_SIZE];
#pragma data_section ".data_imu3"
	v4nm32f dst[NMGL_SIZE];

#define SIZE_ONE 32*32
extern "C" void merge_v4nm32f(float* src1, float* src2, float* src3, float* src4, v4nm32f* dst, int count);
int main()
{
	
	int count=32;

	clock_t t0,t1;

	unsigned int crc = 0;
	for(int i=0;i<NMGL_SIZE;i++){
		src1[i] = i;
		src2[i] = 2*i;
		src3[i] = 3*i;
		src4[i] = 4*i;
		dst[i].vec[0] = 0;
		dst[i].vec[1] = 0;
		dst[i].vec[2] = 0;
		dst[i].vec[3] = 0;
	}

	merge_v4nm32f(src1, src2, src3, src4, dst, NMGL_SIZE);
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 4; j++) {
			
		}
	}
	
	
	return crc;
}
