#include "hal_target.h"
#include "hal.h"
#include <time.h>
#include <nmpp.h>
#include "demo3d_nm0.h"
#include "cache.h"
#include "ringbuffer.h"

#include "nmgl.h"
#include "nmgl_data0.h"

#define CHECK_EXIT0 if(nmglSynchro->exit_nm==EXIT) {	break;	}

SECTION(".data_shared")	Polygons polyArray[COUNT_POLYGONS_BUFFER];


void synchroInit(Synchro* nmglSynchro, int widthImage, int heightImage) {
	nmglSynchro->exit_nm = 0;
	nmglSynchro->counter_nmc0 = 0;
	nmglSynchro->counter_nmc1 = 0;
	nmglSynchro->widthWindow = widthImage;
	nmglSynchro->heightWindow = widthImage;
	nmglSynchro->hasInstrHost = 0;
	nmglSynchro->head = 0;
	nmglSynchro->tail = 0;
	nmglSynchro->time0 = 0;
	nmglSynchro->time1 = 0;
}

#pragma code_section ".text_demo3d"
int nmglvsNm0Init()
{
	halSetProcessorNo(0);
	int fromHost = halHostSync(0xC0DE0000);		// send handshake to host
	if (fromHost != 0xC0DE0086) {					// get  handshake from host
		return -1;
	}
	
	halSetActiveHeap(0);

	nmglSynchro = (Synchro*)halMalloc32(sizeof32(Synchro));
	nmglPolygonsRB = (HalRingBuffer*)halMalloc32(sizeof32(HalRingBuffer));
	//Структура для общения процессорных ядер
	synchroInit(nmglSynchro, WIDTH_IMAGE, HEIGHT_IMAGE);

	//Массив Polygons-структур
	halSyncAddr((int*)nmglSynchro, 1);
	halRingBufferInit(nmglPolygonsRB, polyArray, sizeof32(Polygons), COUNT_POLYGONS_BUFFER, 0, 0, 0);

	//Адрес кольцевого буфера Polygons-структур на nmc1
	halSyncAddr((int*)nmglPolygonsRB, 1);


	// Check memory allocation
	if (nmglSynchro == 0) {
		halHostSync(0xDEADB00F);	// send error to host
		return -1;
	}
	else {
		halHostSync(0x600DB00F);	// send ok to host
	}


	//sync3
	halHostSync((int)nmglSynchro);

	//sync4
	halHostSync((int)0x600d600d);
	halOpenDMA();

} 

