#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "nmtype.h"
#include "utility.h"

typedef union 
{
    float* f;
    int* i;
    unsigned int* ui;
    v4nm32f* v4f;
    v3nm32f* v3f;
    v2nm32f* v2f;  
} PoolPointer;


#endif //__CONTEXT_H__