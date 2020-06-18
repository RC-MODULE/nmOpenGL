#include <time.h>
#include "hal.h"
#include "hal_target.h"
#include "nmpp.h"
#include "demo3d_nm1.h"
#include "stdio.h"
#include "ringbuffer.h"
#include "nmprofiler.h"
#include "cache.h"
#include "nmgl_data1.h"
#include "nmprofiler.h"

#include "nmgl.h"

SECTION(".data_imu1")	int pool0[SIZE_BANK];
SECTION(".data_imu3")	int pool1[SIZE_BANK];

SECTION(".data_imu2")	int segImage[WIDTH_SEG * HEIGHT_SEG];
SECTION(".data_imu2")	int segZBuff[WIDTH_SEG * HEIGHT_SEG];

SECTION(".data_imu0") Vector2 ptrnInnPoints[NMGL_SIZE];
SECTION(".data_imu0") Size ptrnSizes[NMGL_SIZE];
SECTION(".data_shmem1") nm32s valuesZ[NMGL_SIZE];
SECTION(".data_shmem1") nm32s valuesC[NMGL_SIZE];

SECTION(".data_shmem1") nm32s* ppSrcPackPtrns[3 * NMGL_SIZE];
SECTION(".data_shmem1") nm32s* ppDstPackPtrns[3 * NMGL_SIZE];
SECTION(".data_shmem1") nm32s nSizePtrn32[3 * NMGL_SIZE];
SECTION(".data_shmem1") nm32s* zBuffPoints[NMGL_SIZE];
SECTION(".data_shmem1") nm32s* imagePoints[NMGL_SIZE];


SECTION(".data_shmem1") nm32s colorClearBuff[WIDTH_SEG * HEIGHT_SEG];
SECTION(".data_shmem1") nm32s depthClearBuff[WIDTH_SEG * HEIGHT_SEG];


int exitNM1 = 0;

template<class T> T* myMallocT(int size) {
	T* result = (T*)halMalloc32(size * sizeof32(T));

	if (result == 0) throw -2;
	return result;
}

template<class T> T* myMallocT() {
	T* result = (T*)halMalloc32(sizeof32(T));
	if (result == 0) throw -2;
	return result;
}

SECTION(".text_nmglvs") int nmglvsNm1Init()
{
	halSleep(100);
	halSetProcessorNo(1);
	//---------- start nm program ------------

	try {
		int fromHost = halHostSync(0xC0DE0001);		// send handshake to host
		if (fromHost != 0xC0DE0086) {					// get  handshake from host
			throw -1;
		}
		
		setHeap(11);
		cntxt.patterns = myMallocT<PatternsArray>();

		setHeap(13);
		cntxt.imagesData = myMallocT<ImageData>();
		cntxt.imagesData->init();

		setHeap(11);
		DepthImage* depthImage = myMallocT<DepthImage>();

		cntxt.colorBuffer.init(cntxt.imagesData->ptrHead(), WIDTH_IMAGE, HEIGHT_IMAGE);
		cntxt.depthBuffer.init(depthImage, WIDTH_IMAGE, HEIGHT_IMAGE);
	}
	catch (int &e){
		if (e == -2) {
			halHostSync((int)0xDEADB00F);	// send error allocation memory to host
		}
		return e;
	}

	NMGLSynchroData* synchroData = (NMGLSynchroData*)halSyncAddr((int*)cntxt.patterns, 0);
	cntxt.synchro.init(synchroData);
	cntxt.polygonsData = (PolygonsArray*)halSyncAddr(0, 0);

	halHostSync(0x600DB00F);	// send ok to host

	msdInit();
	
#ifdef __GNUC__
	halInstrCacheEnable();
#ifdef PROFILER1
	PROFILER_START();
#endif // PROFILER1	
#endif // __GNUC__
	cntxt.smallColorBuff.init(segImage, WIDTH_SEG, HEIGHT_SEG);
	cntxt.smallDepthBuff.init(segZBuff, WIDTH_SEG, HEIGHT_SEG);

	cntxt.smallClearColorBuff.init(colorClearBuff, WIDTH_SEG, HEIGHT_SEG);
	cntxt.smallClearDepthBuff.init(depthClearBuff, WIDTH_SEG, HEIGHT_SEG);
	
	halSleep(10);

	//sync0
	halHostSync((int)cntxt.patterns);

	cntxt.buffer0 = pool0;
	cntxt.buffer1 = pool1;

	for (int j = 0; j < SMALL_SIZE; j++) {
		int off = j * sizeof32(Pattern);
		cntxt.ppPtrns1_2s[j] = (Pattern*)nmppsAddr_32s((nm32s*)pool0, off);
		cntxt.ppPtrns2_2s[j] = (Pattern*)nmppsAddr_32s((nm32s*)pool1, off);
		cntxt.ppPtrnsCombined_2s[j] = cntxt.polyImgTmp + j;
		cntxt.minusOne[j] = -1;
	}
	//sync3
	halHostSync((int)cntxt.imagesData);

	cntxt.ptrnInnPoints = ptrnInnPoints;
	cntxt.ptrnSizes = ptrnSizes;
	cntxt.valuesZ = valuesZ;
	cntxt.valuesC = valuesC;

	cntxt.ppSrcPackPtrns = ppSrcPackPtrns;
	cntxt.ppDstPackPtrns = ppDstPackPtrns;
	cntxt.nSizePtrn32 = nSizePtrn32;

	cntxt.zBuffPoints = zBuffPoints;
	cntxt.imagePoints = imagePoints;

	cntxt.t0 = clock();
	return 0;
} 

