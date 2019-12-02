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
#include "ringbuffer_host.h"
#include <thread>



using namespace std;

#define SIZE_BUFFER 2
unsigned char srcImg[4 * SIZE_BUFFER * WIDTH_IMAGE * HEIGHT_IMAGE];


HalHostRingBuffer hostImageRB;
HalRingBuffer imagesRB;

void download() {
	while (true) {
		S_VS_MouseStatus mouseStatus;
		VS_GetMouseStatus(&mouseStatus);
		while (halRingBufferIsFull(&imagesRB)) {
			halSleep(2);
		}
		//if (mouseStatus.nKey == VS_MOUSE_LBUTTON) {
		if (mouseStatus.nKey != VS_MOUSE_LBUTTON) {
			halHostRingBufferPop(&hostImageRB, halRingBufferHead(&imagesRB), 1);
		}
		else {
			while (halHostRingBufferIsEmpty(&hostImageRB)) {
				halSleep(2);
			}
			hostImageRB.tail++;
			halWriteMemBlock(&hostImageRB.tail, hostImageRB.remoteTailAddr, 1, hostImageRB.processor);
		}
		imagesRB.head++;
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
	int nmImageRB = halSync(4, 1);
	nmppsFree(patterns);
	halHostRingBufferInit(&hostImageRB, nmImageRB,1);
	ok = halRingBufferInit(&imagesRB, srcImg, hostImageRB.size, SIZE_BUFFER, 0, 0, 0);
	//nmc0, sync4
	
	thread downloadThread(download);
	downloadThread.detach();
	
	halSync(0, 0);

	return 0;
};
