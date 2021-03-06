//------------------------------------------------------------------------
//  Demostration for MAX 2017 
//
//  Author: Alexander Bolornikov
//
//  Copyright (c) 2017 RC Module Inc.
//------------------------------------------------------------------------

#include "VShell.h"
#include "hal.h"
#include "hal_host.h"
#include "math.h"
#include "nmpp.h"
#include "demo3d_host.h"
#include "demo3d_nm1.h"
#include "ringbuffert.h"
#include "imagebuffer.h"



using namespace std;

ImageConnector hostImageRB;

void*  writeMem(const void* src, void* dst, unsigned int size32) {
	int ok = halWriteMemBlock((void*)src, (int)dst, size32, 1);
	return 0;
}

void*  readMem(const void* src, void* dst, unsigned int size32) {
	int ok = halReadMemBlock(dst, (int)src, size32, 1);
	return 0;
}

int nmglvsHostInit()
{
	if (halOpen(PROGRAM, PROGRAM1, NULL)){
		printf("Connection to mc12101 error!\n");
		return -1;
	}

	for (int i = 0; i < NUM_NM_CORES; i++) {
		int handshake = halSync(0xC0DE0086, i);
		if (handshake != (0xC0DE0000 | i)) {
			printf("Handshake with mc12101-nmc%d error!\n", i);
			return -1;
		}
	}
	for (int i = 0; i < NUM_NM_CORES; i++) {
		int ok = halSync(0, i);
		if (ok != 0x600DB00F) {
			printf("Memory nmc%d allocation error!\n", i);
			return -2;
		}
	}
	for (int i = 0; i < NUM_NM_CORES; i++) {
		int ok = halSync(0, i);
		if (ok != 0x600DB00F) {
			printf("Interprocessor allocation error!\n", i);
			return -3;
		}
	}
//----------------init-nmc1------------------------------
	//nmc1, sync0
	int patternsNM = halSyncAddr(0, 1);
	//PatternsArray* patterns = (PatternsArray*)halMalloc32(sizeof32(PatternsArray));
	PatternsArray* patterns = (PatternsArray*)nmppsMalloc_32s(sizeof32(PatternsArray));
	hostCreatePatterns(patterns);
	halWriteMemBlock(patterns, patternsNM, sizeof32(PatternsArray), 1);
	nmppsFree(patterns);
//----------------init-ringbuffer-------------
	//nmc1, sync3
	ImageData* nmImageRB = (ImageData*)halSyncAddr(0, 1);

	hostImageRB.init(nmImageRB, writeMem, readMem);
	
	//nmc0, sync4
	halSync(0, 0);

	return 0;
};
