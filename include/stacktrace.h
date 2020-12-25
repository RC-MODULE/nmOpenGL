#ifndef __STACK_TRACE_H__
#define __STACK_TRACE_H__
#include "ringbuffert.h"
#include "nmprofiler.h"

#define STACK_TRACE_POINT_COUNT 2048


typedef HalRingBufferData<TraceData, STACK_TRACE_POINT_COUNT> StackTraceData;
typedef HalRingBufferConnector<TraceData, STACK_TRACE_POINT_COUNT> StackTraceConnector;



#endif //__SERVICE_H__
