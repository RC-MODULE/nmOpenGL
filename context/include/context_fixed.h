#ifndef __CONTEXT_FIXED_H__
#define __CONTEXT_FIXED_H__

#include "context.h"
#include "utility.h"

#define POOLS_COUNT 3
#define POOL_SIZE 12 * 1024

typedef struct{
    PoolPointer pools[POOLS_COUNT];
} NMGL_CoreContextFixed;


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

	NMGL_CoreContextFixed *getCoreContextFixed(void);

	void context_fixed_init(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__CONTEXT_FIXED_H__