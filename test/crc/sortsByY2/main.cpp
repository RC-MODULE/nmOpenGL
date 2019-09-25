#include "nmpp.h"
#include "time.h"
#include <stdio.h>

#define NMGL_SIZE 1024
#define MAX_STEP 4

#pragma data_section ".data_imu1"
#pragma data_section ".data_imu2"
	float srcAxy[2 *NMGL_SIZE+2];
#pragma data_section ".data_imu3"
	float srcBxy[2 * NMGL_SIZE+2];
#pragma data_section ".data_imu4"
#pragma data_section ".data_imu5"
	int temp[2 * NMGL_SIZE + 2];
	
	
extern "C" void sortByY2(float* srcAxy, 
								  float* srcBxy, 
								  int size);

int main()
{
	clock_t t0,t1;
	nmppsRand_32f(srcAxy, 2 * NMGL_SIZE, 1, 10000);
	nmppsRand_32f(srcBxy, 2 * NMGL_SIZE, 1, 10000);
/*	printf("srcAxy=%f,%f\n", srcAxy[0], srcAxy[1]);
	printf("srcBxy=%f,%f\n", srcBxy[0], srcBxy[1]);
	nmppsConvert_32f32s_rounding(srcAxy, temp, 0, 2 * NMGL_SIZE);
	nmppsConvert_32s32f(temp, srcAxy, 2 * NMGL_SIZE);
	nmppsConvert_32f32s_rounding(srcAxy, temp, 0, 2 * NMGL_SIZE);
	nmppsConvert_32s32f(temp, srcBxy, 2 * NMGL_SIZE);*/
	for(int i=0;i<36;i++){
		printf("y_a,b,c[%d]=%8.2f, ", i, srcAxy[2*i+1]);
		printf("%8.2f, \n", srcBxy[2*i+1]);
	}
	printf("\n");
	unsigned crc = 0;
	//for (int size = 0; size < NMGL_SIZE; size++) {
		int size = 36;
		t0 = clock();
		sortByY2(srcAxy, srcBxy, size);
		nmppsCrcAcc_32f(srcAxy, 4, size + 2, &crc);
		nmppsCrcAcc_32f(srcBxy, 4, size + 2, &crc);
		t1 = clock();
	//}
	
	for(int i=0;i<36;i++){
		printf("y_a,b,c[%d]=%8.2f, ", i, srcAxy[2*i+1]);
		printf("%8.2f\n, ", srcBxy[2*i+1]);
	}
	printf("crc=0x%x\n", crc);
	return t1-t0;
}
