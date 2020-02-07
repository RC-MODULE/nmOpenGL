
#include "stdio.h"
#include "time.h"
#include "hal.h"
#include "demo3d_common.h"

 #define     SIZE    0x400


 extern "C" {


__attribute__ ((section(".data_imu1"))) volatile double x11[SIZE*4];
__attribute__ ((section(".data_imu2"))) volatile double x22[SIZE];
__attribute__ ((section(".data_imu3"))) volatile double y11[SIZE*6];
__attribute__ ((section(".data_imu4"))) volatile double y22[SIZE];


#include "malloc.h"

int main()
{
	printf( "XXXXX\t" );
	
    nmc_malloc_set_heap(4);  
    double *x1 = (double *)malloc(SIZE * sizeof(double));
    nmc_malloc_set_heap(3);
    double *x2 = (double *)malloc(SIZE * sizeof(double));
    nmc_malloc_set_heap(2);
    double *y1 = (double *)malloc(SIZE * sizeof(double));
    nmc_malloc_set_heap(1);
    double *y2 = (double *)malloc(SIZE * sizeof(double));
	nmc_malloc_set_heap(0);
	double *z1 = (double *)malloc(SIZE * sizeof(double));

	printf("x1=0x%x\n", x1);
	printf("x2=0x%x\n", x2);
	printf("y1=0x%x\n", y1);
	printf("y2=0x%x\n", y2);
	printf("z1=0x%x\n", z1);
	
    free(x1);
    free(x2);
    free(y1);
    free(y2);
	free(z1);

	halSleep(10);
    int i;
    nmc_malloc_set_heap(3);
    for (i=0; x1 && i<10; i++){
    	x1= (double *)malloc( SIZE * sizeof(double));
    }
	printf( "XXXXX\n" );

    return i;
}

};

