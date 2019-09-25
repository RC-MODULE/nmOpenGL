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

#pragma data_section ".data_imu0"

#pragma data_section ".data_imu1"
	int src[16384/8];
		
#pragma data_section ".data_imu2"
	int values[32];
	int sizes[32];
	int dst_ref[16384];
	
#pragma data_section ".data_imu3"
	int dst[16384];

#define SIZE_ONE 32*32

int main()
{
	
	int count=32;
	printf("src=0x%x\n",src);
	//printf("matr=0x%x\n",&tmp);
	printf("values=0x%x\n",values);
	printf("dst=0x%x\n",dst);

	clock_t t0,t1;

	unsigned int crc = 0;
	unsigned int crc1 = 0;
	for(int i=0;i<count;i++){
		sizes[i] = SIZE_ONE;
		values[i]=3;
	}
	int min = 15000;
	nmppsRandUniform_32s((nm32s*)src,16384/8);
	nmppsSet_32s((nm32s*)dst, 0xCCCCCCCC, 16384);
	nmppsSet_32s((nm32s*)dst_ref, 0xCCCCCCCC, 16384);
	for(int c=1;c<=32;c*=2){
		printf("%d triangles\n",c);
		for(int s=1;s<=1024;s*=2){
			for(int i=0;i<32;i++){
				sizes[i]=s;
			}
			t0=clock();
			MulCV_2s16s((nm2s*)src,(nm16s*)dst,values,sizes,c);
			t1=clock();
			nmppsCrcAcc_32u((nm32u*)dst,16384,&crc);
			//nmppsCrcAcc_32u((nm32u*)dst_ref,16384,&crc1);
			printf("    size=%4d, time(theory)=%2f, time(practice)=%5d, ECE=%3f%%\n",sizes[0],(sizes[0]*0.25f)*c,t1-t0,(sizes[0]*0.25)/(t1-t0)*100*c);
		}
	}
	/*for(int c=0;c<32;c++){
		printf("%2d treangles\n         ",c);
		printf("width")
		for(int i=0;i<32;i++){
			printf("      %2d",i);
		}
		printf("/nheight ");
		for(int h=0;h<32;h++){
			printf(" %2d",h);
			for(int w=0;w<32;w++){
				t0=clock();
				MulCV_2s16s((nm2s*)src,(nm16s*)dst,values,sizes,c);
				t1=clock();
				nmppsCrcAcc_32u((nm32u*)dst,16384,&crc);
				nmppsCrcAcc_32u((nm32u*)dst_ref,16384,&crc1);
				printf(" %4d",t1-t0);
			}
			printf("/n       ",h);
		}
	}*/
	//! \fn void nmppsAnd_64u(nm64u*,nm64u*,nm64u*,int)

	printf("t1-t0=%u\n",t1-t0);
	printf("crc=0x%x\n",crc);
	printf("crc_ref=0x%x\n",crc1);

	return crc1-crc;
}
