#include <time.h>
#include "hal.h"
#include "hal_target.h"
#include "nmpp.h"
#include "demo3d_nm1.h"
#include "stdio.h"
#include "ringbuffer.h"
#include "nmprofiler.h"
#include "cache.h"
#include "nmprofiler.h"

#include "nmgl.h"

SECTION(".data_imu1")	int pool0[SIZE_BANK];
SECTION(".data_imu3")	int pool1[SIZE_BANK];

SECTION(".data_imu2")	int segImage[WIDTH_SEG * HEIGHT_SEG];
SECTION(".data_imu2")	int segZBuff[WIDTH_SEG * HEIGHT_SEG];

SECTION(".data_imu0") Vector2 ptrnInnPoints[POLYGONS_SIZE];
SECTION(".data_imu0") Size ptrnSizes[POLYGONS_SIZE];
SECTION(".data_shmem1") nm32s valuesZ[POLYGONS_SIZE];
SECTION(".data_shmem1") nm32s valuesC[POLYGONS_SIZE];

SECTION(".data_shmem1") nm32s* ppSrcPackPtrns[3 * POLYGONS_SIZE];
SECTION(".data_shmem1") nm32s* ppDstPackPtrns[3 * POLYGONS_SIZE];
SECTION(".data_shmem1") nm32s nSizePtrn32[3 * POLYGONS_SIZE];
SECTION(".data_shmem1") nm32s* zBuffPoints[POLYGONS_SIZE];
SECTION(".data_shmem1") nm32s* imagePoints[POLYGONS_SIZE];


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

SECTION(".data_imu0A") NMGL_Context_NM1 context;
SECTION(".data_imu0") NMGL_Context_NM1 *NMGL_Context_NM1::context;

SECTION(".text_nmglvs") int nmglvsNm1Init()
{
	halSleep(100);
	halSetProcessorNo(1);
	//---------- start nm program ------------
	NMGL_Context_NM1 *cntxt;
	try {
		int fromHost = halHostSync(0xC0DE0001);		// send handshake to host
		if (fromHost != 0xC0DE0086) {					// get  handshake from host
			throw -1;
		}
		setHeap(0);
		NMGL_Context_NM1::bind(&context);
		cntxt = NMGL_Context_NM1::getContext();
		cntxt->polygonsConnectors = myMallocT<PolygonsConnector>();
		
		setHeap(11);
		cntxt->patterns = myMallocT<PatternsArray>();
		
		setHeap(13);
		cntxt->imagesData = myMallocT<ImageData>();
		cntxt->imagesData->init();

		setHeap(11);
		DepthImage* depthImage = myMallocT<DepthImage>();

		cntxt->colorBuffer.init(cntxt->imagesData->ptrHead(), WIDTH_IMAGE, HEIGHT_IMAGE);
		cntxt->depthBuffer.init(depthImage, WIDTH_IMAGE, HEIGHT_IMAGE);
	}
	catch (int &e){
		if (e == -2) {
			halHostSync((int)0xDEADB00F);	// send error allocation memory to host
		}
		return e;
	}

	halHostSync(0x600DB00F);	// send ok to host

	NMGLSynchroData* synchroData = (NMGLSynchroData*)halSyncAddr((int*)cntxt->patterns, 0);
	cntxt->synchro.init(synchroData);
	PolygonsArray* polygonsData = (PolygonsArray*)halSyncAddr(0, 0);
	cntxt->polygonsConnectors[0].init(polygonsData);

	halHostSync(0x600DB00F);	// send ok to host

	msdInit();
	
#ifdef __GNUC__
	halInstrCacheEnable();
#ifdef PROFILER1
	PROFILER_START();
#endif // PROFILER1	
#endif // __GNUC__
	cntxt->smallColorBuff.init(segImage, WIDTH_SEG, HEIGHT_SEG);
	cntxt->smallDepthBuff.init(segZBuff, WIDTH_SEG, HEIGHT_SEG);

	cntxt->smallClearColorBuff.init(colorClearBuff, WIDTH_SEG, HEIGHT_SEG);
	cntxt->smallClearDepthBuff.init(depthClearBuff, WIDTH_SEG, HEIGHT_SEG);

	//sync0
	halHostSync((int)cntxt->patterns);

	cntxt->buffer0 = pool0;
	cntxt->buffer1 = pool1;

	for (int j = 0; j < SMALL_SIZE; j++) {
		int off = j * sizeof32(Pattern);
		cntxt->ppPtrns1_2s[j] = (Pattern*)nmppsAddr_32s((nm32s*)pool0, off);
		cntxt->ppPtrns2_2s[j] = (Pattern*)nmppsAddr_32s((nm32s*)pool1, off);
		cntxt->ppPtrnsCombined_2s[j] = cntxt->polyImgTmp + j;
		cntxt->minusOne[j] = -1;
	}
	//sync3
	halHostSync((int)cntxt->imagesData);

	cntxt->ptrnInnPoints = ptrnInnPoints;
	cntxt->ptrnSizes = ptrnSizes;
	cntxt->valuesZ = valuesZ;
	cntxt->valuesC = valuesC;

	cntxt->ppSrcPackPtrns = ppSrcPackPtrns;
	cntxt->ppDstPackPtrns = ppDstPackPtrns;
	cntxt->nSizePtrn32 = nSizePtrn32;

	cntxt->zBuffPoints = zBuffPoints;
	cntxt->imagePoints = imagePoints;

	cntxt->t0 = clock();
	return 0;
} 

