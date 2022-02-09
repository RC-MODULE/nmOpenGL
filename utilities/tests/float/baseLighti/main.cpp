#include "nmpp.h"
#include "debugprint.h"
#include "section-hal.h"
#include "utility_float.h"


#define SIZE 1024

INSECTION(".data_imu0") v4nm32f ambient = {0,1,2,3};
INSECTION(".data_imu0") v4nm32f diffuse = {3,2,1,0};
INSECTION(".data_imu0") v4nm32f specular = {2,2,2,2};
INSECTION(".data_imu1") v2nm32f nv[SIZE];
INSECTION(".data_imu2") v2nm32f nh_in_srm[SIZE];
INSECTION(".data_imu3") v4nm32f dst[SIZE + 2];

void testSize(){

}

void testValue(){
	for(int i=0;i< SIZE;i++){
		nv[i].v0 = 4;
		nv[i].v1 = 4;
		nh_in_srm[i].v0 = 0.5;
		nh_in_srm[i].v1 = 0.5;
	}
	for(int size = 0; size < SIZE; size++){
		//t0 = clock();
		baseLighti(&ambient, nv, &diffuse, nh_in_srm, &specular, dst, size);
		//t1 = clock();
		//nmppsCrcAcc_32f((float*)dst, 2, 4 * size + 2, &crc);
	}
	for(int i=0; i < 4; i++){
		//if (dst[0].vec[0] != 13) DEBUG_PLOG_ERROR('%f!=13\n', dst[0].vec[0]);
		//if (dst[0].vec[1] != 10) DEBUG_PLOG_ERROR('%f!=10\n', dst[0].vec[1]);
		//if (dst[0].vec[2] != 7) DEBUG_PLOG_ERROR('%f!=7\n', dst[0].vec[2]);
		//if (dst[0].vec[3] != 4) DEBUG_PLOG_ERROR('%f!=4\n', dst[0].vec[3]);
	}
	DEBUG_PLOG_LEVEL_1("dst={%f,%f,%f,%f}\n", dst[0].vec[0], dst[0].vec[1], dst[0].vec[2], dst[0].vec[3]);
	//DEBUG_PLOG_LEVEL_1("crc=0x%x\n", crc);
}


int main()
{
	testValue();
	return 0;
}
