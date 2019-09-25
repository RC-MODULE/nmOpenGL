#include "malloc32.h"
#include <time.h>
#include "hal.h"
#include "hal_target.h"
#include "nmpp.h"
#include "demo3d_nm1.h"
#include "stdio.h"
#include "ringbuffer.h"
#include "nmprofiler.h"
#include "cache.h"

#include "nmgl.h"
#include "nmgl_data1.h"

extern HalRingBuffer* nmglPolygonsRB;
extern Polygons polyArray[COUNT_POLYGONS_BUFFER];
extern Patterns patterns;
extern int imageArray[COUNT_IMAGE_BUFFER * WIDTH_IMAGE * HEIGHT_IMAGE];
extern HalRingBuffer imageRB;
int exitNM1 = 0;

//#pragma code_section ".text_demo3d"
int nmglvsNm1Init()
{
	halSetProcessorNo(1);
	//---------- start nm program ------------
	int fromHost=ncl_hostSync(0xC0DE0001);		// send handshake to host
	if (fromHost!=0xC0DE0086){					// get  handshake from host
		return -1;
	}
	int ok=0;
	// Check memory allocation

	//Структура для общения процессорных ядер
	nmglSynchro = (Synchro*)halSyncAddr((int*)0, 0);

	//Адрес кольцевого буфера Polygons-структур на nmc0 
	nmglPolygonsRB = (HalRingBuffer*)halSyncAddr(0, 0);

	if (false) {
		halHostSync((int)0);	// send error to host
		return -1;
	}
	else
		halHostSync(0x600DB00F);	// send ok to host

	halOpenDMA();

	halRingBufferInit(&imageRB, imageArray, WIDTH_IMAGE*HEIGHT_IMAGE, COUNT_IMAGE_BUFFER, 0, 0, 0);
	//sync0
	halHostSync((int)&patterns);

	for (int i = 0; i < NMGL_SIZE / SMALL_SIZE; i++) {
		for (int j = 0; j < SMALL_SIZE; j++) {
			int off = j * WIDTH_PTRN*HEIGHT_PTRN / 16;
			ppDstPackPtrnsCA[i * SMALL_SIZE + j] = nmppsAddr_32s((nm32s*)trianglesMul_32s, off);
			ppDstPackPtrnsCB[i * SMALL_SIZE + j] = nmppsAddr_32s((nm32s*)trianglesZMask_32s, off);
		}
	}
	for (int j = 0; j < SMALL_SIZE; j++) {
		int off = j * WIDTH_PTRN*HEIGHT_PTRN / 16;
		ppPatterns1_2s[j] = nmppsAddr_32s((nm32s*)trianglesMul_32s, off);
		ppPatterns2_2s[j] = nmppsAddr_32s((nm32s*)trianglesZMask_32s, off);
		ppTriangles_2s[j] = nmppsAddr_32s(triangles_2s, off);
		minusOne[j] = -1;
	}
	//sync3
	halHostSync((int)&imageRB);
	return 0;
} 

