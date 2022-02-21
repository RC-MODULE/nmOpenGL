#include "context_fixed.h"
#include "nmdata.h"


DATA_IMU1 static int mem_imu0[POOL_SIZE];
DATA_IMU2 static int mem_imu1[POOL_SIZE];
DATA_IMU3 static int mem_imu2[POOL_SIZE];
DATA_IMU3 static int mem_imu3[POOL_SIZE];
DATA_IMU2 static int mem_imu4[POOL_SIZE];

#ifdef __NM__
DATA_IMU0 static NMGL_CoreContextFixed context;
__attribute__((constructor)) void context_fixed_init(void){
    context.pools[0].i = mem_imu0;
    context.pools[1].i = mem_imu1;
    context.pools[2].i = mem_imu2;
    context.pools[3].i = mem_imu3;
    context.pools[4].i = mem_imu4;
}
#endif

NMGL_CoreContextFixed *getCoreContextFixed(void){
#ifdef __NM__
    return &context;
#endif
#ifdef _MSC_VER 
    static NMGL_CoreContextFloat context;
    context.pools[0].f = mem_imu0;
    context.pools[1].f = mem_imu1;
    context.pools[2].f = mem_imu2;
    context.pools[3].f = mem_imu3;
    context.pools[4].f = mem_imu4;
    return &context;
#endif
}

// __attribute__((destructor)) void context_float_free(void){
    
// }
