#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"

#define SIZE 1024
#define MAX_STEP 4

SECTION(".data_imu1") float n_dot_VP[2*SIZE];
SECTION(".data_imu2") v4nm32f srcDiffuse = {1,2,3,4};
SECTION(".data_imu3") v4nm32f srcAmbient = {10,20,30,40};
SECTION(".data_imu4") v4nm32f dstVec[SIZE+2];
	
	
extern "C" void dotMulC_AddC_v4nm32f(float* n_dot_VP, v4nm32f* srcDiffuse, v4nm32f* srcAmbient, v4nm32f* dst, int size);

int main()
{
	for(int i=0;i<SIZE;i++){
		n_dot_VP[2*i] = i;
		n_dot_VP[2*i+1] = i;
	}
	for(int i=0;i<SIZE+2;i++){
		dstVec[i].vec[0] = 0;
		dstVec[i].vec[1] = 0;
		dstVec[i].vec[2] = 0;
		dstVec[i].vec[3] = 0;
	}
	for(int i=0;i<8;i++){
		printf("n_dot_VP[%d]=%.1f\n", i, n_dot_VP[2*i+0]);
	}
	printf("srcDiffuse = %.1f, ", srcDiffuse.vec[0]);
	printf("%.1f, ", srcDiffuse.vec[1]);
	printf("%.1f, ", srcDiffuse.vec[2]);
	printf("%.1f\n", srcDiffuse.vec[3]);
	
	printf("srcAmbient = %.1f, ", srcAmbient.vec[0]);
	printf("%.1f, ", srcAmbient.vec[1]);
	printf("%.1f, ", srcAmbient.vec[2]);
	printf("%.1f\n", srcAmbient.vec[3]);
	
	clock_t t0,t1;
	unsigned crc =0;
	for(int size=2;size<= SIZE;size+=2){
		t0=clock();
		dotMulC_AddC_v4nm32f(n_dot_VP, &srcDiffuse, &srcAmbient, dstVec, size);
		t1=clock();
		nmppsCrcAcc_32f((float*)dstVec, 2, 4*size+2,&crc);
	}
	for(int i=0;i<8;i++){
		printf("dstVec[%d]=%.1f ", i, dstVec[i].vec[0]);
		printf("dstVec[%d]=%.1f ", i, dstVec[i].vec[1]);
		printf("dstVec[%d]=%.1f ", i, dstVec[i].vec[2]);
		printf("dstVec[%d]=%.1f\n", i, dstVec[i].vec[3]);
	}
	printf("crc=0x%x\n", crc);
	return t1-t0;
}
