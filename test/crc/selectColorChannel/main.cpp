#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"

#pragma data_section ".data_imu0"

#pragma data_section ".data_imu1"
		
#pragma data_section ".data_imu1"
	int src[1024];
	
#pragma data_section ".data_imu3"
	int dst[1024+2];


extern "C" void selectColorChannel(v4nm8s* srcImage, int channel, nm8s* dst, int size);

int main()
{
	clock_t t0,t1;
	unsigned crc =0;
	nmppsRandUniform_32s(src,1024);
	nmppsSet_32s(dst, 0xCDCDCDCD, 1024+2);
	for(int channel =0; channel < 4; channel++){
		for(int size=8;size<=1024;size+=8){
			//printf("size=%d\n", size);
			t0 = clock();
			selectColorChannel((v4nm8s*)src, channel, (nm8s*)dst, size);
			t1 = clock();
			nmppsCrcAcc_32s(dst, size/4+2, &crc);
			//printf("crc=0x%x\n", crc);
		}	
	}
	printf("crc=0x%x\n", crc);
	return t1-t0;
	//return 0 ;
}
