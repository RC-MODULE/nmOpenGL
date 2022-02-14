#include "nmpp.h"
#include "debugprint.h"
#include "section-hal.h"
#include "utility_float.h"
#include "uassert.h"


#define SIZE 1024
#define INIT_DST_VALUE -1

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

	DEBUG_PLOG_LEVEL_1("size=%d\n", 1);

	//for(int size = 0; size < SIZE; size++){
		baseLighti(&ambient, nv, &diffuse, nh_in_srm, &specular, dst, 1);
	//}

	v4nm32f dst_ref = {13, 10, 7, 4};
	for(int i=0; i < 4; i++){
		uassert(dst[0].vec[i] == dst_ref.vec[i]);
		// if (dst[0].vec[i] != dst_ref.vec[i]) { 
		// 	DEBUG_PLOG_ERROR("%f!=%f\n", dst[0].vec[i], dst_ref.vec[i]); 	
		// 	DEBUG_PLOG_LEVEL_1("dst={%f,%f,%f,%f}\n", dst[0].vec[0], dst[0].vec[1], dst[0].vec[2], dst[0].vec[3]);
		// 	return;
		// }
	}
	DEBUG_PLOG_LEVEL_0("testValue OK\n");
}


int main()
{
	DEBUG_PLOG_FILE();
	testValue();
	return 0;
}
