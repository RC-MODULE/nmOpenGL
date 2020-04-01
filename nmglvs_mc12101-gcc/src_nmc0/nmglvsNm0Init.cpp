#include "hal_target.h"
#include "hal.h"
#include <time.h>
#include <nmpp.h>
#include "demo3d_nm0.h"
#include "cache.h"
#include "ringbuffer.h"
#include "nmprofiler.h"

#include "nmgl.h"
#include "nmgl_data0.h"

#define CHECK_EXIT0 if(nmglSynchro->exit_nm==EXIT) {	break;	}

SECTION(".data_imu1")	float nmglBuffer0[12 * NMGL_SIZE];
SECTION(".data_imu2")	float nmglBuffer1[12 * NMGL_SIZE];
SECTION(".data_imu3")	float nmglBuffer2[12 * NMGL_SIZE];
SECTION(".data_imu4")	float nmglBuffer3[12 * NMGL_SIZE];


SECTION(".data_imu1")	float x0[NMGL_SIZE];
SECTION(".data_imu2")	float y0[NMGL_SIZE];
SECTION(".data_imu3")	float x1[NMGL_SIZE];
SECTION(".data_imu4")	float y1[NMGL_SIZE];
SECTION(".data_imu5")	float x2[NMGL_SIZE];
SECTION(".data_imu6")	float y2[NMGL_SIZE];
SECTION(".data_imu7")	int z_int[NMGL_SIZE];
SECTION(".data_imu6")	v4nm32s lightsValues[NMGL_SIZE];


template<class T> inline T* myMallocT() {
	T* result = (T*)halMalloc32(sizeof32(T));
	if (result == 0) throw -2;
	return result;
}

template<class T> inline T* myMallocT(int count) {
	T* result = (T*)halMalloc32(count * sizeof32(T));
	if (result == 0) throw -2;
	return result;
}

SECTION(".text_nmglvs") int nmglvsNm0Init()
{
	halSetProcessorNo(0);
	halSleep(100);
	NMGLSynchroData* synchroData;
	try {
		int fromHost = halHostSync(0xC0DE0000);		// send handshake to host
		if (fromHost != 0xC0DE0086) {					// get  handshake from host
			throw -1;
		}

		setHeap(8);
		synchroData = myMallocT<NMGLSynchroData>();
		synchroData->init();
		setHeap(10);
		cntxt.polygonsData = myMallocT<PolygonsArray>();
		cntxt.polygonsData->init();
		cntxt.init(synchroData);

	}
	catch (int& e) {
		if (e == -2) {
			halHostSync(0xDEADB00F);
		}
		return e;
	}

	cntxt.patterns = (PatternsArray*)halSyncAddr(synchroData, 1);
	halSyncAddr(cntxt.polygonsData, 1);

	halHostSync(0x600DB00F);	// send ok to host

	cntxt.trianInner.x0 = x0;
	cntxt.trianInner.y0 = y0;
	cntxt.trianInner.x1 = x1;
	cntxt.trianInner.y1 = y1;
	cntxt.trianInner.x2 = x2;
	cntxt.trianInner.y2 = y2;
	cntxt.trianInner.z = z_int;
	cntxt.trianInner.colors = lightsValues;
	cntxt.trianInner.maxSize = NMGL_SIZE;
	cntxt.trianInner.size = 0;


	cntxt.buffer0 = nmglBuffer0;
	cntxt.buffer1 = nmglBuffer1;
	cntxt.buffer2 = nmglBuffer2;
	cntxt.buffer3 = nmglBuffer3;


#ifdef __GNUC__
	halDmaInitC();
	halInstrCacheEnable();
#ifdef PROFILER0
	nmprofiler_init();
#endif // PROFILER0	
#endif // __GNUC__

	//sync4
	halHostSync((int)0x600d600d);

	nmglViewport(0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);
	return 0;
} 

