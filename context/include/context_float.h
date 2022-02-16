#ifndef __CONTEXT_FLOAT_H__
#define __CONTEXT_FLOAT_H__

#include "utility.h"
#include "context.h"

#define POOLS_COUNT 6
#define POOL_SIZE 12 * 1024

typedef struct{

	PoolPointer pools[POOLS_COUNT];

	v4nm32f temp;


} NMGL_CoreContextFloat;

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

	NMGL_CoreContextFloat *getCoreContextFloat(void);

	void context_float_init(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__CONTEXT_FLOAT_H__
