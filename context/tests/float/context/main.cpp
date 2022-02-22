#include "context_float.h"
#include "uassert.h"

//__attribute__((section(".data_imu0"))) static NMGL_CoreContextFloat context;
//__attribute__((section(".bss.data_imu1"))) static float mem_imu0[POOL_SIZE];
//__attribute__((section(".bss.data_imu2"))) static float mem_imu1[POOL_SIZE];
//__attribute__((section(".bss.data_imu3"))) static float mem_imu2[POOL_SIZE];
//__attribute__((section(".bss.data_imu4"))) static float mem_imu3[POOL_SIZE];
//__attribute__((section(".bss.data_imu5"))) static float mem_imu4[POOL_SIZE];
//__attribute__((section(".bss.data_imu7"))) static float mem_imu5[POOL_SIZE];

int lowerLimits[] = { 0x8000 , 0x10000, 0x18000, 0x20000, 0x28000, 0x38000 };
int upperLimits[] = { 0x10000, 0x18000, 0x20000, 0x28000, 0x30000, 0x40000 };

int main(){
    NMGL_CoreContextFloat *context = getCoreContextFloat();

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