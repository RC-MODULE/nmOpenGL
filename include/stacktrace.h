#ifndef __STACK_TRACE_H__
#define __STACK_TRACE_H__
#include "ringbuffert.h"

#define STACK_TRACE_POINT_COUNT 2

typedef struct s_StackPoint{
	int time;
	int direction;
	int address;
	int dummy;
} StackPoint;


typedef HalRingBufferData<StackPoint, STACK_TRACE_POINT_COUNT> StackTraceData;
typedef HalRingBufferConnector<StackPoint, STACK_TRACE_POINT_COUNT> StackTraceConnector;



#endif //__SERVICE_H__
