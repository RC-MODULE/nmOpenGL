#include "nmtype.h"
extern "C" {
	void baseAddrOffs_32s(nm32s* baseAddr, int* offsets, nm32s** ppDst, int size) {
		for (int i = 0; i < size; i++) {
			ppDst[i] = baseAddr + offsets[i];
		}
	}

	void baseAddrOffs_32u(nm32u* baseAddr, int* offsets, nm32u** ppDst, int size) {
		for (int i = 0; i < size; i++) {
			ppDst[i] = baseAddr + offsets[i];
		}
	}

	void baseAddrOffs_16s(nm16s* baseAddr, int* offsets, nm16s** ppDst, int size) {
		for (int i = 0; i < size; i++) {
			ppDst[i] = baseAddr + offsets[i];
		}
	}

	void baseAddrOffs_16u(nm16u* baseAddr, int* offsets, nm16u** ppDst, int size) {
		for (int i = 0; i < size; i++) {
			ppDst[i] = baseAddr + offsets[i];
		}
	}
}