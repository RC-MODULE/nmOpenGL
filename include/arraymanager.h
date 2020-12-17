/*!
 *  \file arraymanager.h
 *  \author Zhilenkov Ivan
 */


#ifndef __ADDR_MANAGER_H__
#define __ADDR_MANAGER_H__

#include "nmdef.h"
#include "hal.h"
#include "nmblas.h"

template <typename T>
void addrManagerDefaultCopy(const void* src, void* dst, size_t size) {
	//halCopyRISC(src, dst, sizeof32(size));
	nmblas_scopy(sizeof32(size), (float*)src, 1, (float*)dst, 1);
}

/*!
 *  \brief Класс последовательной выгрузки данных из большого массива в более маленький и ограниченный массив
 *  \author Жиленков Иван
 */
template <typename T>
class ArrayManager{
private:
	int offset;
	int maxLocalSize;
	int totalSize;
	T* startAddr;
	T* localAddr;
	void (*copyFunction)(const void *, void *, size_t);
public:

	ArrayManager() : copyFunction(addrManagerDefaultCopy<T>){
		
	}

	void set(T* arrayVec, int fullSize, int maxPartSize, void(*function)(const void *, void *, size_t)) {
		startAddr = arrayVec;
		totalSize = fullSize;
		maxLocalSize = maxPartSize;
		if (function != 0) {
			copyFunction = function;
		}
		reset();
	}

	int pop(T* dst) {
		int localSize = MIN(totalSize - offset, maxLocalSize);
		localSize = MAX(0, localSize);
		copyFunction(localAddr, dst, localSize);
		localAddr += localSize;
		offset += localSize;
		return localSize;
	}

	bool isEmpty() {
		return offset >= totalSize;
	}

	void reset() {
		localAddr = startAddr;
		offset = 0;
	}
	
};

#endif
