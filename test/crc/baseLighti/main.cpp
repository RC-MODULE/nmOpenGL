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

#define SIZE 1024

SECTION("demo_imu0") v4nm32f ambient = {0,1,2,3};
SECTION("demo_imu0") v4nm32f diffuse = {3,2,1,0};
SECTION("demo_imu0") v4nm32f specular = {2,2,2,2};
SECTION("demo_imu1") v2nm32f nv[SIZE];
SECTION("demo_imu2") v2nm32f nh_in_srm[SIZE];
SECTION("demo_imu3") v4nm32f dst[SIZE + 2];


int main()
{
	
	int count=32;

	clock_t t0,t1;
	unsigned crc=0;
	
	for(int i=0;i< SIZE;i++){
		nv[i].v0 = 4;
		nv[i].v1 = 4;
		nh_in_srm[i].v0 = 0.5;
		nh_in_srm[i].v1 = 0.5;
	}
	for(int size = 0; size < SIZE; size++){
		t0 = clock();
		baseLighti(&ambient, nv, &diffuse, nh_in_srm, &specular, dst, size);
		t1 = clock();
		nmppsCrcAcc_32f((float*)dst, 2, 4 * size + 2, &crc);
	}
	printf("dst={%f,%f,%f,%f}\n", dst[0].vec[0], dst[0].vec[1], dst[0].vec[2], dst[0].vec[3]);
	printf("crc=0x%x\n", crc);
	return t1-t0;
}
