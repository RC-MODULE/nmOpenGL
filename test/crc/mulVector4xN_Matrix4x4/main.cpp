#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"

#pragma data_section ".data_imu0"

#pragma data_section ".data_imu1"
	float srcMatrix4x4[16]={0.0f,1.0f,2.0f,3.0f,
							4.0f,5.0f,6.0f,7.0f,
							8.0f,9.0f,10.0f,11.0f,
							12.0f,13.0f,14.0f,15.0f};
		
#pragma data_section ".data_imu2"
	float src[4096];
	
#pragma data_section ".data_imu3"
	float dst[4096+2];

#define SIZE_ONE 11*32


extern "C" void mul_v4nm32f_mat4nm32f(float* srcVector4xN, float* srcMatrix4x4, float* dstVector4xN, int count);

int main()
{

	clock_t t0,t1;

	unsigned int crc = 0;
	//nmppsRand_32f(src, 4096, 0.1f, 1000.0f);
	for(int i=0;i<4096+2;i++){
		dst[i]=1.0f;
	}
	for(int i=0;i<4096;i++){
		src[i]=i;
	}
/*	for(int i=0;i<16;i+=4){
		for(int j=0;j<4;j++){
			printf("%f ",srcMatrix4x4[i+j]);
		}
		printf("\n");
	}
	printf("\n");
	for(int i=0;i<16;i++){
		printf("%f\n",src[i]);
	}
	printf("\n");
	for(int i=0;i<16;i++){
		printf("%f\n",dst[i]);
	}
	printf("\n");*/
	for(int size=0;size <= 1024;size+=2){
		t0 = clock();
		mul_v4nm32f_mat4nm32f(src, srcMatrix4x4, dst, size);
		t1 = clock();
		nmppsCrcAcc_32f(dst, 16, size+2, &crc);
	}
	for(int i=0;i<16;i++){
		printf("%f\n",dst[i]);
	}

	printf("time=%d\n",t1-t0);
	printf("crc=0x%x\n",crc);
	return crc;
}
