#include <time.h>
#include "hal.h"
#include "hal_target.h"
#include "nmpp.h"
#include "demo3d_nm1.h"
#include "stdio.h"
#include "ringbuffer.h"
#include "cache.h"
#include "link.h"
#include "pattern.h"
#include "command_nm1.h"

#include "nmgl.h"

SECTION(".data_imu1")	static int pool0[SIZE_BUFFER_NM1];
SECTION(".data_imu1")	static Pattern patternsPack[POLYGONS_SIZE];
SECTION(".data_imu0")	static Pattern* ppPatternsPack[POLYGONS_SIZE];
SECTION(".data_imu2")	static int pool1[SIZE_BUFFER_NM1];
SECTION(".data_imu3")	static int pool2[SIZE_BUFFER_NM1];

SECTION(".data_imu3")	static int segImage[WIDTH_SEG * HEIGHT_SEG];
SECTION(".data_imu2")	static int segZBuff[WIDTH_SEG * HEIGHT_SEG];

SECTION(".data_imu0") static Vector2 ptrnInnPoints[POLYGONS_SIZE];
SECTION(".data_imu0") static Size ptrnSizes[POLYGONS_SIZE];
SECTION(".data_shmem1") nm32s imageOffsets[POLYGONS_SIZE];

//TEXTURING_PART
SECTION(".data_imu0") static float x0_nmgl[POLYGONS_SIZE];
SECTION(".data_imu0") static float y0_nmgl[POLYGONS_SIZE];
SECTION(".data_imu0") static float x1_nmgl[POLYGONS_SIZE];
SECTION(".data_imu0") static float y1_nmgl[POLYGONS_SIZE];
SECTION(".data_imu0") static float x2_nmgl[POLYGONS_SIZE];
SECTION(".data_imu0") static float y2_nmgl[POLYGONS_SIZE];

SECTION(".data_imu0") static float texS0[POLYGONS_SIZE];
SECTION(".data_imu0") static float texT0[POLYGONS_SIZE];
SECTION(".data_imu0") static float texS1[POLYGONS_SIZE];
SECTION(".data_imu0") static float texT1[POLYGONS_SIZE];
SECTION(".data_imu0") static float texS2[POLYGONS_SIZE];
SECTION(".data_imu0") static float texT2[POLYGONS_SIZE];

SECTION(".data_imu0") static float w0[POLYGONS_SIZE];
SECTION(".data_imu0") static float w1[POLYGONS_SIZE];
SECTION(".data_imu0") static float w2[POLYGONS_SIZE];



void readColorBufferDMA(void* data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);
void writeColorBufferDMA(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);
void readDepthBufferDMA(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);
void writeDepthBufferDMA(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);
void readColorBufferRISC(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);
void writeColorBufferRISC(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);
void readDepthBufferRISC(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);
void writeDepthBufferRISC(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);

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

SECTION(".data_imu0A") NMGL_Context_NM1 nmglContext;
SECTION(".data_imu0") NMGL_Context_NM1 *NMGL_Context_NM1::context;


SECTION(".text_nmglvs") int nmglvsNm1Init()
{
	halSleep(500);


	halSetProcessorNo(1);
	//---------- start nm program ------------
	NMGL_Context_NM1 *cntxt;
	ImageData* imagesData;
	try {
		int fromNm0 = halSync(0xC0DE0001, 0);		// send handshake to host
		if (fromNm0 != 0xC0DE0000) {					// get  handshake from host
			throw -1;
		}
		setHeap(0);
		NMGL_Context_NM1::bind(&nmglContext);
		cntxt = NMGL_Context_NM1::getContext();
		
		NMGL_Context *globalContext = (NMGL_Context*)halSyncAddr(0, 0);
		NMGL_MakeCurrentContext(globalContext);

		NMGLSynchroData* synchroData = (NMGLSynchroData*)halSyncAddr(0, 0);
		nmglSynchro.init(synchroData);


		
		cntxt->texState.palette_pointers[0] = (NMGLubyte *)halSyncAddr(0, 0);
		cntxt->texState.paletts_widths_pointers[0] = (unsigned int*)halSyncAddr(0, 0);
		globalContext->polygon.stipple.pattern = (NMGLubyte *)halSyncAddr(0, 0);

		setHeap(11);
		cntxt->patterns = myMallocT<PatternsArray>();

		setHeap(11);
		hostCreatePatterns(cntxt->patterns);
		halSleep(10);


		setHeap(11);
		
		cntxt->frameConnector.readColor = readColorBufferDMA;
		cntxt->frameConnector.writeColor = writeColorBufferDMA;
		cntxt->frameConnector.readDepth = readDepthBufferDMA;
		cntxt->frameConnector.writeDepth = writeDepthBufferDMA;
		cntxt->texState.init();
		cntxt->init_elements();
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
	halSync(0x600DB00F, 0);	// send ok to host

	
	
#ifdef __GNUC__
	halInstrCacheEnable();
#endif // __GNUC__
	msdInit();
	NMGL_FrameBufferInit(&cntxt->innerFramebuffer, WIDTH_SEG, HEIGHT_SEG);
	cntxt->innerFramebuffer.buffers[0] = segImage;
	cntxt->innerFramebuffer.buffers[2] = segZBuff;	


	cntxt->buffer0 = pool0;
	cntxt->buffer1 = pool1;
	cntxt->buffer2 = pool2;
	cntxt->buffers[0].init(pool0, SIZE_BUFFER_NM1);
	cntxt->buffers[1].init(pool1, SIZE_BUFFER_NM1);
	cntxt->buffers[2].init(pool2, SIZE_BUFFER_NM1);

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

	cntxt->t0 = clock();

//TEXTURING_PART
	cntxt->x0 = x0_nmgl;
	cntxt->y0 = y0_nmgl;
	cntxt->x1 = x1_nmgl;
	cntxt->y1 = y1_nmgl;
	cntxt->x2 = x2_nmgl;
	cntxt->y2 = y2_nmgl;

	cntxt->texS0 = texS0;
	cntxt->texT0 = texT0;
	cntxt->texS1 = texS1;
	cntxt->texT1 = texT1;
	cntxt->texS2 = texS2;
	cntxt->texT2 = texT2;

	cntxt->w0 = w0;
	cntxt->w1 = w1;
	cntxt->w2 = w2;


	halSync(0x600D600D, 0);


	return 0;
} 

