#include "context_float.h"

#define POOL_SIZE 1024

__attribute__((section(".data_imu0"))) static NMGL_CoreContextFloat context;
__attribute__((section(".data_imu1"))) static float mem_imu0[POOL_SIZE];
__attribute__((section(".data_imu2"))) static float mem_imu1[POOL_SIZE];
__attribute__((section(".data_imu3"))) static float mem_imu2[POOL_SIZE];
__attribute__((section(".data_imu4"))) static float mem_imu3[POOL_SIZE];
__attribute__((section(".data_imu5"))) static float mem_imu4[POOL_SIZE];
__attribute__((section(".data_imu7"))) static float mem_imu5[POOL_SIZE];

__attribute__((constructor)) void context_float_init(void){
    context.pools[0].f = mem_imu0;
    context.pools[1].f = mem_imu1;
    context.pools[2].f = mem_imu2;
    context.pools[3].f = mem_imu3;
    context.pools[4].f = mem_imu4;
    context.pools[5].f = mem_imu5;
}

NMGL_CoreContextFloat *getCoreContextFloat(void){
    return &context;
}

// __attribute__((destructor)) void context_float_free(void){
    
// }