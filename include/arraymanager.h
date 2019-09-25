#ifndef __ADDR_MANAGER_H__
#define __ADDR_MANAGER_H__

#include "hal.h"

template <typename T>
void addrManagerDefaultCopy(const T* src, T* dst, size_t size) {
	halCopyRISC(src, dst, sizeof32(size));
}

template <typename T>
class ArrayManager{
private:
	int remaningSize;
	int maxLocalSize;
	int totalSize;
	T* startAddr;
	T* localAddr;
	void (*copyFunction)(const T *, T *, size_t);
public:

	ArrayManager() : copyFunction(addrManagerDefaultCopy<T>){
		
	}

	void set(T* arrayVec, int fullSize, int maxPartSize, void(*function)(const T *, T *, size_t)) {
		startAddr = arrayVec;
		totalSize = fullSize;
		maxLocalSize = maxPartSize;
		if (function != 0) {
			copyFunction = function;
		}
		reset();
	}

	int pop(T* dst) {
		int localSize = MIN(remaningSize, maxLocalSize);
		localSize = MAX(0, localSize);
		copyFunction(localAddr, dst, localSize);
		localAddr += localSize;
		remaningSize -= localSize;
		return localSize;
	}

	bool isEmpty() {
		return remaningSize <= 0;
	}

	void reset() {
		localAddr = startAddr;
		remaningSize = totalSize;
	}
	
};

#endif
