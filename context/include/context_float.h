#ifndef __CONTEXT_FLOAT_H__
#define __CONTEXT_FLOAT_H__

#include "utility.h"
#include "context.h"


typedef struct{

	PoolPointer *pools;
	int poolsCount;
	int poolSize32;

	v4nm32f temp;


} NMGL_CoreContextFloat;

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

	NMGL_CoreContextFloat *getCoreContextFloat(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__CONTEXT_FLOAT_H__