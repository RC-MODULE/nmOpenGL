#include "context_float.h"
#include "nmdata.h"


DATA_IMU1 static float mem_imu0[POOL_SIZE];
DATA_IMU2 static float mem_imu1[POOL_SIZE];
DATA_IMU3 static float mem_imu2[POOL_SIZE];
DATA_IMU4 static float mem_imu3[POOL_SIZE];
DATA_IMU5 static float mem_imu4[POOL_SIZE];
DATA_IMU7 static float mem_imu5[POOL_SIZE];

#ifdef __NM__
DATA_IMU0 static NMGL_CoreContextFloat context;
__attribute__((constructor)) void context_float_init(void){
    context.pools[0].f = mem_imu0;
    context.pools[1].f = mem_imu1;
    context.pools[2].f = mem_imu2;
    context.pools[3].f = mem_imu3;
    context.pools[4].f = mem_imu4;
    context.pools[5].f = mem_imu5;
}
#endif

NMGL_CoreContextFloat *getCoreContextFloat(void){
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
    context.pools[5].f = mem_imu5;
    return &context;
#endif
    return 0;
}

// __attribute__((destructor)) void context_float_free(void){
    
// }