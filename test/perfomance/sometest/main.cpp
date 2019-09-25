#include "nmpp.h"
#include "time.h"
#include "stdlib.h"
#include "demo3d_nm0.h"
#include <stdio.h>

#define NMGL_SIZE 1024

#pragma data_section ".data_imu0"
	long long imu0[NMGL_SIZE/2];
#pragma data_section ".data_imu1"
	long long imu1[NMGL_SIZE/2];
#pragma data_section ".data_imu2"
	long long imu2[NMGL_SIZE/2];
#pragma data_section ".data_imu3"
	long long imu3[NMGL_SIZE/2];
#pragma data_section ".data_imu4"
	long long imu4[NMGL_SIZE/2];
#pragma data_section ".data_imu5"
	long long imu5[NMGL_SIZE/2];
#pragma data_section ".data_imu6"
	long long imu6[NMGL_SIZE/2];
#pragma data_section ".data_imu7"


int main()
{
	return 0;
}
