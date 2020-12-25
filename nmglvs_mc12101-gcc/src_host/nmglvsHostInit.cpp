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


#include <thread>
#include "stacktrace.h"



using namespace std;

ImageConnector hostImageRB; 
StackTraceConnector stackTraceConnector;

bool gccmap_address2symbol_(char* mapfile, unsigned addr, char* fullname) {
	FILE* f;
	char str[1024];
	f = fopen(mapfile, "rt");
	if (f == 0)
		return false;
	char addr_str[16];
	sprintf(addr_str, "%08x", addr);

	while (!feof(f)) {
		fgets(str, 1024, f);
		const char* where_addr = strstr(str, addr_str);
		if (where_addr == 0)
			continue;

		if (strstr(where_addr, "0x"))
			continue;
		while (strstr(where_addr, " "))
			where_addr++;
		//if (strlen(str)>(where_addr - str + 11))
		//	continue;
		sscanf(where_addr, " %s ", fullname);
		fclose(f);
		return true;
	}
	fclose(f);
	return false;
}

void*  writeMem(const void* src, void* dst, unsigned int size32) {
	int ok = halWriteMemBlock((void*)src, (int)dst, size32, 1);
	return 0;
}

void*  readMem(const void* src, void* dst, unsigned int size32) {
	int ok = halReadMemBlock(dst, (int)src, size32, 1);
	return 0;
}


void*  writeMem0(const void* src, void* dst, unsigned int size32) {
	int ok = halWriteMemBlock((void*)src, (int)dst, size32, 0);
	return 0;
}
void*  readMem0(const void* src, void* dst, unsigned int size32) {
	int ok = halReadMemBlock(dst, (int)src, size32, 0);
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
	int ok = halWriteMemBlock(patterns, patternsNM, sizeof32(PatternsArray), 1);
	nmppsFree(patterns);
//----------------init-ringbuffer-------------
	//nmc1, sync3
	ImageData* nmImageRB = (ImageData*)halSyncAddr(0, 1);

	hostImageRB.init(nmImageRB, writeMem, readMem);

	StackTraceData *stackTraceData = (StackTraceData*)halSyncAddr(0, 0);
	stackTraceConnector.init(stackTraceData, writeMem, readMem);
	//nmc0, sync4
	halSync(0, 0);
	return 0;
};
