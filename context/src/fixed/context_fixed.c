#include "context_fixed.h"


__attribute__((section(".data_imu0"))) static NMGL_CoreContextFixed context;
__attribute__((section(".data_imu1"))) static float mem_imu0[SIZE_BUFFER_NM1];
__attribute__((section(".data_imu2"))) static float mem_imu1[SIZE_BUFFER_NM1];
__attribute__((section(".data_imu3"))) static float mem_imu2[SIZE_BUFFER_NM1];
__attribute__((section(".data_imu1"))) Pattern patternsPack[POLYGONS_SIZE];

__attribute__((constructor)) void context_fixed_init(void){
    context.pools[0].f = mem_imu0;
    context.pools[1].f = mem_imu1;
    context.pools[2].f = mem_imu2;
}

NMGL_CoreContextFixed *getCoreContextFixed(void){
    return &context;
}

// __attribute__((destructor)) void context_float_free(void){
    
// }
