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
#include "stdio.h"
#include "stdlib.h"
#include "nmpp.h"
#include "demo3d_host.h"
#include "demo3d_nm1.h"
#include "ringbuffert.h"
#include <thread>



using namespace std;

VshellImageData vshellImages;
VshellImageConnector vshellImagesConnector;


ImageConnector hostImageRB;


inline void*  writeMem(const void* src, void* dst, unsigned int size32) {
	halWriteMemBlock(src, (int)dst, size32, 1);
	return 0;
}

inline void*  readMem(const void* src, void* dst, unsigned int size32) {
	halReadMemBlock(dst, (int)src, size32, 1);
	return 0;
}


void download() {
	while (true) {
		S_VS_MouseStatus mouseStatus;
		VS_GetMouseStatus(&mouseStatus);
		while (vshellImagesConnector.isFull()) {
			halSleep(2);
		}
#if defined(PROFILER0) || defined(PROFILER1)
		if (mouseStatus.nKey == VS_MOUSE_LBUTTON) {
#else
		if (mouseStatus.nKey != VS_MOUSE_LBUTTON) {
#endif
			//halHostRingBufferPop(&hostImageRB, vshellImagesConnector.ptrHead(), 1);
			hostImageRB.pop(vshellImagesConnector.ptrHead(), 1);
		}
		else {
			while (hostImageRB.isEmpty()) {
				halSleep(2);
			}
			hostImageRB.incTail();
		}
		(*vshellImagesConnector.pHead)++;
	}
}


int nmglvsHostInit()
{
	if (halOpen(PROGRAM, PROGRAM1)){
		printf("Connection to mc12101 error!");
		return -1;
	}
	printf("\n");

	int handshake= halSync(0xC0DE0086,0);
	if (handshake!=0xC0DE0000){
		printf("Handshake with mc12101-nmc0 error!");
		return -1;
	}
	handshake = halSync(0xC0DE0086, 1);
	if (handshake != 0xC0DE0001) {
		printf("Handshake with mc12101-nmc1 error!");
		return -1;
	}	

	int ok=halSync(0, 0);	// Get	status of memory allocation from nm
	if (ok!=0x600DB00F){
		printf("Memory nmc0 allocation error!");
		return -1;
	}
	ok = halSync(0, 1);	// Get	status of memory allocation from nm
	if (ok != 0x600DB00F) {
		printf("Memory nmc1 allocation error!");
		return -1;
	}
//----------------init-nmc1------------------------------
	//nmc1, sync0
	int patternsNM = halSync(1,1);
	Patterns* patterns = (Patterns*)nmppsMalloc_32s(sizeof32(Patterns));
	hostCreatePatterns(patterns);
	ok = halWriteMemBlock(patterns, patternsNM, sizeof32(Patterns), 1);
//----------------init-ringbuffer-------------
	//nmc1, sync3
	ImageData* nmImageRB = (ImageData*)halSync(4, 1);
	nmppsFree(patterns);

	hostImageRB.init(nmImageRB, writeMem, readMem);
	vshellImagesConnector.init(&vshellImages);
	
	thread downloadThread(download);
	downloadThread.detach();
	
	//nmc0, sync4
	halSync(0, 0);

	return 0;
};
