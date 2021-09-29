//------------------------------------------------------------------------
//  Demostration for MAX 2017 
//
//  Author: Alexander Bolornikov
//
//  Copyright (c) 2017 RC Module Inc.
//------------------------------------------------------------------------

#include "hal.h"
#include "hal_host.h"
#include "demo3d_host.h"
#include "imagebuffer.h"
#include <thread>

#ifdef STACK_TRACE_ENABLED
#include "stacktrace.h"
StackTraceConnector stackTraceConnector;
#endif



using namespace std;

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

int framebufferAddr;

int nmglvsHostInit()
{
	if (halOpen(PROGRAM, PROGRAM1, NULL)){
		printf("Connection to mc12101 error!\n");
		return -1;
	}

	int handshake = halSync(0xC0DE0086, 0);
	if (handshake != 0xC0DE0000) {
		printf("Handshake with mc12101-nmc0 error!\n");
		return -1;
	}

	framebufferAddr = halSyncAddr(0, 0);
	NMGL_Framebuffer framebuffer;
	halReadMemBlock(&framebuffer, framebufferAddr, sizeof32(NMGL_Framebuffer), 0);
	return 0;
};
