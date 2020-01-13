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

SECTION(".data_shmem1") nm32s* ppSrcPackPtrns[3 * NMGL_SIZE];
SECTION(".data_shmem1") nm32s* ppDstPackPtrns[3 * NMGL_SIZE];
SECTION(".data_shmem1") nm32s nSizePtrn32[3 * NMGL_SIZE];
SECTION(".data_shmem1") nm32s* zBuffPoints[NMGL_SIZE];
SECTION(".data_shmem1") nm32s* imagePoints[NMGL_SIZE];



int exitNM1 = 0;

template<class T> T* myMallocT(int size) {
	return (T*)halMalloc32(size * sizeof32(T));
}

template<class T> T* myMallocT() {
	return (T*)halMalloc32(sizeof32(T));
}

SECTION(".text_nmglvs") int nmglvsNm1Init(NMGL_Context_NM1& cntxt)
{
	halSleep(100);
	halSetProcessorNo(1);
	//---------- start nm program ------------
	int fromHost=ncl_hostSync(0xC0DE0001);		// send handshake to host
	if (fromHost!=0xC0DE0086){					// get  handshake from host
		return -1;
	}
	int ok=0;

	setHeap(11);
	cntxt.patterns = myMallocT<Patterns>();
	

#ifdef __GNUC__
	halInstrCacheEnable();
	halDmaInitC();
#ifdef PROFILER1
	nmprofiler_init();
#endif // PROFILER1	
#endif // __GNUC__

	NMGLSynchroData* synchroData = (NMGLSynchroData*)halSyncAddr((int*)cntxt.patterns, 0);
	cntxt.synchro.init(synchroData);
	cntxt.polygonsData = (PolygonsArray*)halSyncAddr(0, 0);

	ImageData* imagesData = myMallocT<ImageData>();
	cntxt.imagesData = imagesData;
	cntxt.imagesData->init();

	DepthImage32* depthImage = myMallocT<DepthImage32>();
	
	cntxt.colorBuffer.init(imagesData->ptrHead(), WIDTH_IMAGE, HEIGHT_IMAGE);
	cntxt.depthBuffer.init(depthImage, WIDTH_IMAGE, HEIGHT_IMAGE);

	cntxt.colorSegment.set(segImage, WIDTH_SEG, HEIGHT_SEG, msdAdd2D);
	cntxt.depthSegment.set(segZBuff, WIDTH_SEG, HEIGHT_SEG, msdAdd2D);

	halSleep(10);
	if (cntxt.patterns == 0 || imagesData == 0 || depthImage == 0) {
		halHostSync((int)0);	// send error to host
		return -1;
	}
	else
		halHostSync(0x600DB00F);	// send ok to host


	//sync0
	halHostSync((int)cntxt.patterns);

	cntxt.buffer0 = pool0;
	cntxt.buffer1 = pool1;

	for (int j = 0; j < SMALL_SIZE; j++) {
		int off = j * WIDTH_PTRN*HEIGHT_PTRN / 16;
		cntxt.ppPtrns1_2s[j] = nmppsAddr_32s((nm32s*)pool0, off);
		cntxt.ppPtrns2_2s[j] = nmppsAddr_32s((nm32s*)pool1, off);
		cntxt.ppPtrnsCombined_2s_basic[j] = nmppsAddr_32s(cntxt.polyImgTmp, off);
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

	cntxt.t0 = clock();
} 

