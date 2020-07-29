#include <cerrno>
#include <iostream>

#include "nmplv/vSupport.h"
#include "service.h"

extern "C"
void duplicate_16s(const nm16s *srcArray,  nm32s *dstArray, int count)
{
	for(int i = 0;i < count; i++){
		int n = srcArray[i] & 0xFFFF;
		n |= (n << 16);
		dstArray[i] = n;
	}
}
