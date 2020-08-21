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

SECTION(".data_imu0") nm32s offsetTrX[NMGL_SIZE];
SECTION(".data_imu0") nm32s offsetTrY[NMGL_SIZE];
SECTION(".data_imu0") nm32s widths[NMGL_SIZE];
SECTION(".data_imu0") nm32s heights[NMGL_SIZE];
SECTION(".data_shmem1") nm32s valuesZ[NMGL_SIZE];
SECTION(".data_shmem1") nm32s valuesC[NMGL_SIZE];

#ifdef TEXTURE_ENABLED
SECTION(".data_imu0") float x0[NMGL_SIZE];
SECTION(".data_imu0") float y0[NMGL_SIZE];
SECTION(".data_imu0") float x1[NMGL_SIZE];
SECTION(".data_imu0") float y1[NMGL_SIZE];
SECTION(".data_imu0") float x2[NMGL_SIZE];
SECTION(".data_imu0") float y2[NMGL_SIZE];

SECTION(".data_imu0") float texS0[NMGL_SIZE];
SECTION(".data_imu0") float texT0[NMGL_SIZE];
SECTION(".data_imu0") float texS1[NMGL_SIZE];
SECTION(".data_imu0") float texT1[NMGL_SIZE];
SECTION(".data_imu0") float texS2[NMGL_SIZE];
SECTION(".data_imu0") float texT2[NMGL_SIZE];

SECTION(".data_imu0") float zEye0[NMGL_SIZE];
SECTION(".data_imu0") float zEye1[NMGL_SIZE];
SECTION(".data_imu0") float zEye2[NMGL_SIZE];
#endif //TEXTURE_ENABLED


SECTION(".data_shmem1") nm32s* ppSrcPackPtrns[3 * NMGL_SIZE];
SECTION(".data_shmem1") nm32s* ppDstPackPtrns[3 * NMGL_SIZE];
SECTION(".data_shmem1") nm32s nSizePtrn32[3 * NMGL_SIZE];
SECTION(".data_shmem1") nm32s* zBuffPoints[NMGL_SIZE];
SECTION(".data_shmem1") nm32s* imagePoints[NMGL_SIZE];


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
		DepthImage32* depthImage = myMallocT<DepthImage32>();

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
	
	halSleep(10);

	//sync0
	halHostSync((int)cntxt.patterns);

	cntxt.buffer0 = pool0;
	cntxt.buffer1 = pool1;

	for (int j = 0; j < SMALL_SIZE; j++) {
		int off = j * WIDTH_PTRN*HEIGHT_PTRN / 16;
		cntxt.ppPtrns1_2s[j] = (Pattern*)nmppsAddr_32s((nm32s*)pool0, off);
		cntxt.ppPtrns2_2s[j] = (Pattern*)nmppsAddr_32s((nm32s*)pool1, off);
		cntxt.ppPtrnsCombined_2s[j] = cntxt.polyImgTmp + j;
		cntxt.minusOne[j] = -1;
	}
	//sync3
	halHostSync((int)cntxt.imagesData);

	cntxt.offsetTrX = offsetTrX;
	cntxt.offsetTrY = offsetTrY;
	cntxt.widths = widths;
	cntxt.heights = heights;
	cntxt.valuesZ = valuesZ;
	cntxt.valuesC = valuesC;

	cntxt.ppSrcPackPtrns = ppSrcPackPtrns;
	cntxt.ppDstPackPtrns = ppDstPackPtrns;
	cntxt.nSizePtrn32 = nSizePtrn32;

	cntxt.zBuffPoints = zBuffPoints;
	cntxt.imagePoints = imagePoints;

#ifdef TEXTURE_ENABLED
    cntxt.texState.textureEnabled = 1;
    cntxt.x0 = x0;
    cntxt.y0 = y0;
    cntxt.x1 = x1;
    cntxt.y1 = y1;
    cntxt.x2 = x2;
    cntxt.y2 = y2;

    cntxt.texS0 = texS0;
    cntxt.texT0 = texT0;
    cntxt.texS1 = texS1;
    cntxt.texT1 = texT1;
    cntxt.texS2 = texS2;
    cntxt.texT2 = texT2;

    cntxt.zEye0 = zEye0;
    cntxt.zEye1 = zEye1;
    cntxt.zEye2 = zEye2;
#endif //TEXTURE_ENABLED



	cntxt.t0 = clock();
	return 0;
} 

