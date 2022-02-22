#include "context_fixed.h"
#include "uassert.h"

//DATA_IMU1 static int mem_imu0[POOL_SIZE];
//DATA_IMU2 static int mem_imu1[POOL_SIZE];
//DATA_IMU3 static int mem_imu2[POOL_SIZE];
//DATA_IMU3 static int mem_imu3[POOL_SIZE];
//DATA_IMU2 static int mem_imu4[POOL_SIZE];

int lowerLimits[] = { 0x8000 , 0x10000, 0x18000, 0x18000, 0x10000 };
int upperLimits[] = { 0x10000, 0x18000, 0x20000, 0x20000, 0x18000 };

int main(){
    NMGL_CoreContextFixed *context = getCoreContextFixed();

#ifdef __NM__
    for(int i = 0; i < POOLS_COUNT; i++){
        DEBUG_PLOG_LEVEL_1("pools[%d]=%p\n", i, context->pools[i].i);
        uassert((int)context->pools[i].i >= lowerLimits[i]);
        uassert((int)context->pools[i].i < upperLimits[i]);
    }
#endif
#ifdef _MSC_VER
    for(int i = 0; i < POOLS_COUNT; i++){
        DEBUG_PLOG_LEVEL_1("pools[%d]=%p\n", i, context->pools[i].i);
        uassert((int)context->pools[i].i != 0);
        uassert((int)context->pools[i].i != 0);
    }
#endif
    DEBUG_TEST_OK();
    return 0;
}