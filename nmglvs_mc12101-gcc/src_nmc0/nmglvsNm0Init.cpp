#include "hal_target.h"
#include "hal.h"
#include <time.h>
#include <nmpp.h>
#include "demo3d_nm0.h"
#include "cache.h"
#include "ringbuffer.h"

#ifdef STACK_TRACE_ENABLED
#include "stacktrace.h"
#endif //STACK_TRACE_ENABLED

#include "nmgl.h"


#define CHECK_EXIT0 if(nmglSynchro->exit_nm==EXIT) {	break;	}
#ifdef TEST_NMGL_TEX_FUNC
SECTION(".data_imu0") void* cntxtAddr_nm1;
#endif
SECTION(".data_imu1")	float nmglBuffer0[12 * NMGL_SIZE];
SECTION(".data_imu2")	float nmglBuffer1[12 * NMGL_SIZE];
SECTION(".data_imu3")	float nmglBuffer2[12 * NMGL_SIZE];
SECTION(".data_imu4")	float nmglBuffer3[12 * NMGL_SIZE];
SECTION(".data_imu5")	float nmglBuffer4[12 * NMGL_SIZE];
SECTION(".data_imu7")	float nmglBuffer5[12 * NMGL_SIZE];

SECTION(".data_imu2")	float nmglx0[NMGL_SIZE];
SECTION(".data_imu2")	float nmgly0[NMGL_SIZE];
SECTION(".data_imu2")	float nmglx1[NMGL_SIZE];
SECTION(".data_imu3")	float nmgly1[NMGL_SIZE];
SECTION(".data_imu3")	float nmglx2[NMGL_SIZE];
SECTION(".data_imu3")	float nmgly2[NMGL_SIZE];
SECTION(".data_imu4")	int nmglz_int[NMGL_SIZE];
SECTION(".data_imu6")	v4nm32s nmgllightsValues[NMGL_SIZE];
#ifdef TEXTURE_ENABLED
SECTION(".data_imu1")	float nmgls0[NMGL_SIZE];
SECTION(".data_imu2")	float nmglt0[NMGL_SIZE];
SECTION(".data_imu3")	float nmgls1[NMGL_SIZE];
SECTION(".data_imu4")	float nmglt1[NMGL_SIZE];
SECTION(".data_imu5")	float nmgls2[NMGL_SIZE];
SECTION(".data_imu6")	float nmglt2[NMGL_SIZE];
SECTION(".data_imu7")	float nmglw0[NMGL_SIZE];
SECTION(".data_imu7")	float nmglw1[NMGL_SIZE];
SECTION(".data_imu7")	float nmglw2[NMGL_SIZE];
#endif //TEXTURE_ENABLED

SECTION(".data_imu6") int dividedMasksMemory[4][3 * NMGL_SIZE / 32];

SECTION(".data_imu6") int masksBits[36][3 * NMGL_SIZE / 32];

#ifdef STACK_TRACE_ENABLED
SECTION(".data_imu6") StackTraceConnector stackTraceConnector;
extern "C" {
	StackTraceData nmprofiler_trace;
}
#endif //STACK_TRACE_ENABLED

extern  NMGLubyte* mipmap; //texture memory

int counter = 0;

template<class T> inline T* myMallocT() {
	
	T* result = (T*)halMalloc32(sizeof32(T));
	if (result == 0) throw counter;
	counter++;
	return result;
}

template<class T> inline T* myMallocT(int count) {
	T* result = (T*)halMalloc32(count * sizeof32(T));
	if (result == 0) throw counter;
	counter++;
	return result;
}

SECTION(".data_imu0") NMGL_Context_NM0 *NMGL_Context_NM0::context;

SECTION(".text_nmglvs") int nmglvsNm0Init()
{
#if defined(__GNUC__) && defined(DEBUG) && defined(STACK_TRACE_ENABLED)
	nmprofiler_init();
#endif // __GNUC__
#ifdef TEXTURE_ENABLED
	halLedOn(0);
#endif //TEXTURE_ENABLED


	halSetProcessorNo(0);


	NMGLSynchroData* synchroData;
	NMGL_Context_NM0 *cntxt;
	PolygonsArray* polygonsData;


	try {
		int fromHost = halHostSync(0xC0DE0000);		// send handshake to host
		if (fromHost != 0xC0DE0086) {					// get  handshake from host
			return 1;
		}
		setHeap(8);
		synchroData = myMallocT<NMGLSynchroData>();
		synchroData->init();

		setHeap(7);
		NMGL_Context_NM0::create(synchroData);	
		cntxt = NMGL_Context_NM0::getContext();
		cntxt->init(synchroData);

		setHeap(1);
		cntxt->polygonsConnectors = myMallocT<PolygonsConnector>();

		setHeap(10);
		polygonsData = myMallocT<PolygonsArray>();
		polygonsData->init();
		for(int i = 0; i< COUNT_POLYGONS_BUFFER; i++){
			polygonsData->ptr(i)->count = 0;
		}
		cntxt->polygonsConnectors[0].init(polygonsData);

		cntxt->beginEndInfo.vertex = myMallocT<v4nm32f>(BIG_NMGL_SIZE);
		cntxt->beginEndInfo.normal = myMallocT<v4nm32f>(BIG_NMGL_SIZE);
		cntxt->beginEndInfo.color = myMallocT<v4nm32f>(BIG_NMGL_SIZE);
		cntxt->beginEndInfo.inBeginEnd = false;
		cntxt->beginEndInfo.maxSize = BIG_NMGL_SIZE;
#ifdef STACK_TRACE_ENABLED		
		stackTraceConnector.init(&nmprofiler_trace);
#endif //STACK_TRACE_ENABLED
		//Allocate memory for textures.
		//Must be in EMI. 
		//EMI has enough space and does not require address mapping at mc12101
		setHeap(12);
#ifdef TEXTURE_ENABLED
		mipmap = myMallocT<NMGLubyte>(MIPMAP_MEM_SIZE); 
#endif //TEXTURE_ENABLED
	}
	catch (int& e) {
		halHostSync(0xDEADB00F);
		return e;
	}
	halHostSync(0x600DB00F);	// send ok to host
	
	cntxt->patterns = (PatternsArray*)halSyncAddr(synchroData, 1);
	halSyncAddr(polygonsData, 1);
#ifdef TEST_NMGL_TEX_FUNC
	cntxtAddr_nm1 = (void*)halSyncAddr(0, 1);
#ifndef __NM__
	cntxtAddr_nm1 = 0; //static shared memory is not supported in x86 model
#endif //__NM__
       	   
#endif //TEST_NMGL_TEX_FUNC
	halHostSync(0x600DB00F);	// send ok to host

	cntxt->pointRadius = 1;

	cntxt->trianInner.x0 = nmglx0;
	cntxt->trianInner.y0 = nmgly0;
	cntxt->trianInner.x1 = nmglx1;
	cntxt->trianInner.y1 = nmgly1;
	cntxt->trianInner.x2 = nmglx2;
	cntxt->trianInner.y2 = nmgly2;
	cntxt->trianInner.z = nmglz_int;
#ifdef TEXTURE_ENABLED
	cntxt->trianInner.s0 = nmgls0;
	cntxt->trianInner.t0 = nmglt0;
	cntxt->trianInner.s1 = nmgls1;
	cntxt->trianInner.t1 = nmglt1;
	cntxt->trianInner.s2 = nmgls2;
	cntxt->trianInner.t2 = nmglt2;
	cntxt->trianInner.w0 = nmglw0;
	cntxt->trianInner.w1 = nmglw1;
	cntxt->trianInner.w2 = nmglw2;
#endif //TEXTURE_ENABLED
	cntxt->trianInner.colors = nmgllightsValues;
	cntxt->trianInner.maxSize = NMGL_SIZE;
	cntxt->trianInner.size = 0;


	cntxt->lineInner.x0 = nmglx0;
	cntxt->lineInner.y0 = nmgly0;
	cntxt->lineInner.x1 = nmglx1;
	cntxt->lineInner.y1 = nmgly1;
	cntxt->lineInner.z = nmglz_int;
	cntxt->lineInner.colors = nmgllightsValues;
	cntxt->lineInner.maxSize = NMGL_SIZE;
	cntxt->lineInner.size = 0;

	cntxt->pointInner.x0 = nmglx0;
	cntxt->pointInner.y0 = nmgly0;
	cntxt->pointInner.z = nmglz_int;
	cntxt->pointInner.colors = nmgllightsValues;
	cntxt->pointInner.maxSize = NMGL_SIZE;
	cntxt->pointInner.size = 0;


	cntxt->buffer0 = nmglBuffer0;
	cntxt->buffer1 = nmglBuffer1;
	cntxt->buffer2 = nmglBuffer2;
	cntxt->buffer3 = nmglBuffer3;
	cntxt->buffer4 = nmglBuffer4;
	cntxt->buffer5 = nmglBuffer5;

	cntxt->dividedMasks[0].init((nm1*)dividedMasksMemory[0], (nm1*)dividedMasksMemory[1]);
	cntxt->dividedMasks[1].init((nm1*)dividedMasksMemory[2], (nm1*)dividedMasksMemory[3]);


	nmglViewport(0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);
	int countSegs = cntxt->windowInfo.nColumns * cntxt->windowInfo.nRows;
	for (int i = 0; i < countSegs; i++) {
		cntxt->segmentMasks[i].init((nm1*)masksBits[i]);
	}
#ifdef __GNUC__
	halDmaInitC();
	halInstrCacheEnable();
#endif // __GNUC__
#ifdef STACK_TRACE_ENABLED
	halHostSyncAddr(&nmprofiler_trace);
#endif //STACK_TRACE_ENABLED
	//sync4
	halHostSync((int)0x600d600d);
	nmglClearColor(0, 0, 0, 1.0f);
	nmglClearDepthf(1);

	return 0;
} 

