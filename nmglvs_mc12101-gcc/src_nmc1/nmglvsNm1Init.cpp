//#include <time.h>
#include "hal.h"
#include "hal_target.h"
#include "nmpp.h"
#include "demo3d_nm1.h"
#include "stdio.h"
#include "ringbuffer.h"
#include "cache.h"
#include "link.h"
#include "pattern.h"
#include "context_fixed.h"
#include "debugprint.h"

#include "nmgl.h"

//SECTION(".data_imu1")	int pool0[SIZE_BUFFER_NM1];
SECTION(".data_imu1")	Pattern patternsPack[POLYGONS_SIZE];
SECTION(".data_imu0")	Pattern* ppPatternsPack[POLYGONS_SIZE];
//SECTION(".data_imu2")	int pool1[SIZE_BUFFER_NM1];
//SECTION(".data_imu3")	int pool2[SIZE_BUFFER_NM1];

//SECTION(".data_imu3")	int segImage[WIDTH_SEG * HEIGHT_SEG];
//SECTION(".data_imu2")	int segZBuff[WIDTH_SEG * HEIGHT_SEG];

SECTION(".data_imu0") Vector2 ptrnInnPoints[POLYGONS_SIZE];
SECTION(".data_imu0") Size ptrnSizes[POLYGONS_SIZE];
SECTION(".data_shmem1") nm32s imageOffsets[POLYGONS_SIZE];

//TEXTURING_PART
SECTION(".data_imu0") float x0[POLYGONS_SIZE];
SECTION(".data_imu0") float y0[POLYGONS_SIZE];
SECTION(".data_imu0") float x1[POLYGONS_SIZE];
SECTION(".data_imu0") float y1[POLYGONS_SIZE];
SECTION(".data_imu0") float x2[POLYGONS_SIZE];
SECTION(".data_imu0") float y2[POLYGONS_SIZE];

SECTION(".data_imu0") float texS0[POLYGONS_SIZE];
SECTION(".data_imu0") float texT0[POLYGONS_SIZE];
SECTION(".data_imu0") float texS1[POLYGONS_SIZE];
SECTION(".data_imu0") float texT1[POLYGONS_SIZE];
SECTION(".data_imu0") float texS2[POLYGONS_SIZE];
SECTION(".data_imu0") float texT2[POLYGONS_SIZE];

SECTION(".data_imu0") float w0[POLYGONS_SIZE];
SECTION(".data_imu0") float w1[POLYGONS_SIZE];
SECTION(".data_imu0") float w2[POLYGONS_SIZE];


int exitNM1 = 0;

SECTION(".data_imu0") HalRingBufferData<NM_Command, PRIORITY_SIZE> linkSynchro;
SECTION(".data_imu0") HalRingBufferConnector<NM_Command, PRIORITY_SIZE> linkConnector(&linkSynchro);

#define LINK_PORT 2
SECTION(".data_imu0") NMGL_SynchroSlaveRingBuffer nmglSynchro;				///< ��������� ��� ������� ����������� ���� � ������

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

SECTION(".data_imu0") NMGL_Context_NM1 nmglContext;
SECTION(".data_imu0") NMGL_Context_NM1 *NMGL_Context_NM1::context;


SECTION(".text_nmglvs") int nmglvsNm1Init()
{
	halSleep(500);

	halSetProcessorNo(1);
	//---------- start nm program ------------
	NMGL_Context_NM1 *cntxt;
	ImageData* imagesData;
	DEBUG_PLOG_LEVEL_0("nm1 initialization\n");
	try {
		int fromNm0 = halSync(0xC0DE0001, 0);		// send handshake to host
		if (fromNm0 != 0xC0DE0000) {					// get  handshake from host
			throw -1;
		}
		DEBUG_PLOG_LEVEL_0("handshake nm0 passed\n");
		NMGLSynchroData* synchroData = (NMGLSynchroData*)halSyncAddr(0, 0);
		nmglSynchro.init(synchroData);
		DEBUG_PLOG_LEVEL_0("get synchro data\n");

		setHeap(0);
		NMGL_Context_NM1::bind(&nmglContext);
		cntxt = NMGL_Context_NM1::getContext();
		
		cntxt->texState.palette_pointers[0] = (NMGLubyte *)halSyncAddr(0, 0);
		cntxt->texState.paletts_widths_pointers[0] = (unsigned int*)halSyncAddr(0, 0);
		cntxt->polygon.stipple.pattern = (NMGLubyte *)halSyncAddr(0, 0);
		DEBUG_PLOG_LEVEL_0("get textState\n");

		setHeap(11);
		cntxt->patterns = myMallocT<PatternsArray>();
		
		setHeap(13);
		hostCreatePatterns(cntxt->patterns);
		halSleep(10);
		DEBUG_PLOG_LEVEL_0("patterns inited\n");

		cntxt->defaultFramebuffer = (NMGL_Framebuffer *)halSyncAddr(0, 0);

		setHeap(11);
		

		cntxt->colorBuffer.init(cntxt->defaultFramebuffer->buffers[1], WIDTH_IMAGE, HEIGHT_IMAGE);
		cntxt->depthBuffer.init(cntxt->defaultFramebuffer->buffers[2], WIDTH_IMAGE, HEIGHT_IMAGE);
		cntxt->texState.init();
		cntxt->init_elements();
		cntxt->shadeModel=NMGL_SMOOTH;
		DEBUG_PLOG_LEVEL_0("texState inited\n");
	}
	catch (int &e) {
		if (e == -2) {
			halSync((int)0xDEADB00F);	// send error allocation memory to host
		}
		return e;
	}
#ifdef TEST_NMGL_TEX_FUNC
	halSyncAddr((void*)cntxt, 0);	
#endif //TEST_NMGL_TEX_FUNC
	DEBUG_PLOG_LEVEL_0("sync with nm0...");
	halSync(0x600DB00F, 0);	// send ok to host
	DEBUG_PLOG_LEVEL_0("OK\n");
	
#ifdef __GNUC__
	halInstrCacheEnable();
#endif // __GNUC__
	msdInit();
	auto core_context = getCoreContextFixed();

	cntxt->smallColorBuff.init(core_context->pools[3].i, WIDTH_SEG, HEIGHT_SEG);
	cntxt->smallDepthBuff.init(core_context->pools[4].i, WIDTH_SEG, HEIGHT_SEG);

	//cntxt->smallClearColorBuff.init(colorClearBuff, WIDTH_SEG, HEIGHT_SEG);
	//cntxt->smallClearDepthBuff.init(depthClearBuff, WIDTH_SEG, HEIGHT_SEG);
	
	cntxt->buffer0 = core_context->pools[0].i;
	cntxt->buffer1 = core_context->pools[1].i;
	cntxt->buffer2 = core_context->pools[2].i;
	cntxt->buffers[0].init(cntxt->buffer0, SIZE_BUFFER_NM1);
	cntxt->buffers[1].init(cntxt->buffer1, SIZE_BUFFER_NM1);
	cntxt->buffers[2].init(cntxt->buffer2, SIZE_BUFFER_NM1);

	cntxt->patternPack.patterns = patternsPack;
	cntxt->patternPack.origins = ptrnInnPoints;
	cntxt->patternPack.sizes = ptrnSizes;
	cntxt->patternPack.imagePositions = imageOffsets;
	cntxt->patternPack.ppPattern = ppPatternsPack;

	for (int j = 0; j < POLYGONS_SIZE; j++) {
		cntxt->patternPack.ppPattern[j] = cntxt->patternPack.patterns + j;
	}

	for (int j = 0; j < SMALL_SIZE; j++) {
		cntxt->minusOne[j] = -1;
	}
	//sync3
	nmppsCopy_32s(cntxt->patterns->fillTable, cntxt->fillInnerTable, sizeof32(cntxt->patterns->fillTable));

	cntxt->imageOffsets = imageOffsets;

	cntxt->valuesZ = cntxt->buffer2 + 0;
	cntxt->valuesC = cntxt->buffer2 + POLYGONS_SIZE;
	cntxt->zBuffPoints = (nm32s**)(cntxt->buffer2 + 2 * POLYGONS_SIZE);
	cntxt->imagePoints = (nm32s**)(cntxt->buffer2 + 4 * POLYGONS_SIZE);

	//cntxt->t0 = clock();
	cntxt->pointSize = 1;

	DEBUG_PLOG_LEVEL_0("Buffers inited\n");

//TEXTURING_PART
	cntxt->x0 = x0;
	cntxt->y0 = y0;
	cntxt->x1 = x1;
	cntxt->y1 = y1;
	cntxt->x2 = x2;
	cntxt->y2 = y2;

	cntxt->texS0 = texS0;
	cntxt->texT0 = texT0;
	cntxt->texS1 = texS1;
	cntxt->texT1 = texT1;
	cntxt->texS2 = texS2;
	cntxt->texT2 = texT2;

	cntxt->w0 = w0;
	cntxt->w1 = w1;
	cntxt->w2 = w2;

	DEBUG_PLOG_LEVEL_0("sync with nm0...");
	halSync(0x600D600D, 0);
	DEBUG_PLOG_LEVEL_0("OK\n");

	return 0;
} 

