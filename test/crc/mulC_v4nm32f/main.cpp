#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"
#include "demo3d_common.h"

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

#define SIZE 16

SECTION(".data_imu0") v4nm32f srcV[SIZE];
SECTION(".data_imu1") v4nm32f srcC = {0,100,500,1000};		
SECTION(".data_imu2") v4nm32f dst[SIZE];

extern "C" void mulC_v4nm32f(v4nm32f* pSrcC, v4nm32f* pSrcV, v4nm32f* pDst, int size);
int main()
{
	
	int count=32;

	clock_t t0,t1;
	nmppsRand_32f((float*)srcV, 4*SIZE,1,1000);

	unsigned int crc = 0;
	for(int j=0;j<4;j++){
		printf("%f, ",srcC.vec[j]);
	}
	printf("\n\n");
	
	for(int i=0;i<16;i++){
		for(int j=0;j<4;j++){
			printf("%f, ",srcV[i].vec[j]);
		}
		printf("\n");
	}
	printf("\n");

	mulC_v4nm32f(srcV, &srcC, dst, 8);
	for(int i=0;i<16;i++){
		for(int j=0;j<4;j++){
			printf("%f, ",dst[i].vec[j]);
		}
		printf("\n");
	}
	
	
	return crc;
}
