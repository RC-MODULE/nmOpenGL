#include "nmpp.h"
#include "stdio.h"
#include "time.h"
#include "demo3d_common.h"
#include "myserverdma.h"

#define SIZE 1024
#define COUNT 100

SECTION(".data_imu2") float src[SIZE];	

SECTION(".data_imu3") float dst[SIZE +2];



int main()
{
	clock_t t0, t1;
	clock_t innerT0, innerT1;

	t0 = clock();
	halDmaStartC(src, dst, SIZE);
	while (halDmaIsCompleted());
	t1 = clock();
	
	printf("one dma: size=%d,time=%d\n", SIZE, (int)(t1 - t0));
	for (int i = 0; i < SIZE; i++) {
		src[i] = i;
		dst[i] = 0xCDCDCDCD;
	}
	

	msdInit();
	int timeWithoutWait = 0;
	t0 = clock();
	
	for(int i=0; i < COUNT; i++){
		innerT0 = clock();
		int k = msdAdd(src, dst, SIZE);	
		innerT1 = clock();
		timeWithoutWait = MAX(innerT1 - innerT0, timeWithoutWait);
		//printf("%d\n", k);
	}	
	
	t1 = clock();
	printf("%d\n", (int)t1-t0);
	msdWaitDma();
	t1 = clock();
	int time = t1-t0;
	printf("count=%d, without waiting=%d, with waiting=%d\n", COUNT, timeWithoutWait, time / COUNT);
	//printf("msd %d dma: size=%d, mean time=%d\n", COUNT, SIZE, (int)(t1 - t0) / COUNT);

	return t1 - t0;
}
