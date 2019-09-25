#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"

#pragma data_section ".data_imu0"

#pragma data_section ".data_imu1"
	float Vec4C[4]={0.0f,1.0f,2.0f,3.0f};
		
#pragma data_section ".data_imu2"
	float src[4*4096];
	
#pragma data_section ".data_imu3"
	float dst[4096];

#define SIZE_ONE 11*32

//#ifded
extern "C" void dotProdVec4C_Vec4xN(float* srcVector4xN, float* pVec4C, float* dst, int count);

int main()
{

	clock_t t0,t1;

	unsigned int crc = 0;
	nmppsRand_32f(src, 4096, 0.1f, 1000.0f);
	printf("Vec4C=0x%x, src=0x%x, dst=0x%x\n", Vec4C, src, dst);
	for(int size=0;size < 1024;size+=2){
		t0 = clock();
		dotProdVec4C_Vec4xN(src, Vec4C, dst, 4096);
		t1 = clock();
		nmppsCrcAcc_32f(dst, 16, size, &crc);
	}

	printf("time=%d\n",t1-t0);
	return crc;
}
