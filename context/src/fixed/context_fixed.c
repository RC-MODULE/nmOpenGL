#include "context_fixed.h"


__attribute__((section(".data_imu0"))) static NMGL_CoreContextFixed context;
__attribute__((section(".data_imu1"))) static int mem_imu0[POOL_SIZE];
__attribute__((section(".data_imu2"))) static int mem_imu1[POOL_SIZE];
__attribute__((section(".data_imu3"))) static int mem_imu2[POOL_SIZE];
__attribute__((section(".data_imu3"))) static int mem_imu3[POOL_SIZE];
__attribute__((section(".data_imu2"))) static int mem_imu4[POOL_SIZE];
//__attribute__((section(".data_imu1"))) Pattern patternsPack[POLYGONS_SIZE];

__attribute__((constructor)) void context_fixed_init(void){
    context.pools[0].i = mem_imu0;
    context.pools[1].i = mem_imu1;
    context.pools[2].i = mem_imu2;
    context.pools[3].i = mem_imu3;
    context.pools[4].i = mem_imu4;
}

NMGL_CoreContextFixed *getCoreContextFixed(void){
    return &context;
}

// __attribute__((destructor)) void context_float_free(void){
    
// }
