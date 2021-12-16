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

#define PRINT_ADDR(a) printf(#a "=%p\n", a)
#define PRINT_HEX(a) printf(#a "=0x%x\n", a)

//Возможные значения ширины или высоты: 480,640,768,960,1024,1152


SECTION(".data_imo0") NMGL_Context *globalContext;

void NMGL_FrameBufferInit(NMGL_FrameBuffer *fb, int width, int height){
	fb->head = 0;
	fb->tail = 0;
	fb->sizeOfAddr = sizeof(void *);
	fb->sizeOfInt = sizeof(int);
	fb->width = width;
	fb->height = height;
}

void NMGL_ContextInit(NMGL_Context *context)
{
	NMGL_FrameBufferInit(&context->defaultFrameBuffer, WIDTH_IMAGE, HEIGHT_IMAGE);

	context->isUseTwoSidedMode = NMGL_FALSE;
	context->isCullFace = NMGL_FALSE;
	context->cullFaceType = NMGL_BACK;
	context->frontFaceOrientation = NMGL_CCW;
	context->normalizeEnabled = NMGL_FALSE;
	context->pointSize = 1.0f;

	context->lightingInfo.init();

	context->currentMatrixStack = &context->modelviewMatrixStack;
	context->modelviewMatrixStack.base = context->modelviewMatrix;
	context->modelviewMatrixStack.current = 0;
	context->modelviewMatrixStack.size = 16;
	context->modelviewMatrixStack.type = NMGL_MODELVIEW_MATRIX;

	context->projectionMatrixStack.base = context->projectionMatrix;
	context->projectionMatrixStack.current = 0;
	context->projectionMatrixStack.size = 2;
	context->projectionMatrixStack.type = NMGL_PROJECTION_MATRIX;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				context->modelviewMatrix[0].matr[i * 4 + j] = 1.0f;
				context->projectionMatrix[0].matr[i * 4 + j] = 1.0f;
				context->normalMatrix.matr[i * 4 + j] = 1.0;
			}
			else
			{
				context->modelviewMatrix[0].matr[i * 4 + j] = 0.0f;
				context->projectionMatrix[0].matr[i * 4 + j] = 0.0f;
				context->normalMatrix.matr[i * 4 + j] = 0.0f;
			}
		}
	}
	context->normalMatrix.matr[15] = 0.0f;

	context->vertexArray.enabled = NMGL_FALSE;
	context->vertexArray.size = 0;
	context->normalArray.enabled = NMGL_FALSE;
	context->normalArray.size = 0;
	context->colorArray.enabled = NMGL_FALSE;
	context->colorArray.size = 0;

	context->currentColor.vec[0] = (float)1.0;
	context->currentColor.vec[1] = (float)1.0;
	context->currentColor.vec[2] = (float)1.0;
	context->currentColor.vec[3] = (float)1.0;

	context->currentNormal.vec[0] = (float)0.0;
	context->currentNormal.vec[1] = (float)0.0;
	context->currentNormal.vec[2] = (float)1.0;
	context->currentNormal.vec[3] = (float)0.0;

	context->viewport.viewportMulZ = (1 - 0) * 0.5f * ZBUFF_MAX;
	context->viewport.viewportAddZ = (1 + 0) * 0.5f * ZBUFF_MAX;

	context->shadeModel = NMGL_SMOOTH;

	context->point.smooth_enabled = NMGL_FALSE;
	context->line.smooth_enabled = NMGL_FALSE;
	context->line.stipple.enabled = NMGL_FALSE;
	context->line.width = 1.0;
	context->line.stipple.factor = 1;
	context->line.stipple.pattern = 0xFFFF;

	context->polygon.stipple.enabled = NMGL_FALSE;

	context->polygon.offset_fill_enabled = NMGL_FALSE;

	context->scissorTest.isEnabled = NMGL_FALSE;

	context->alpha_test.enabled = NMGL_FALSE;
	context->alpha_test.func = NMGL_ALWAYS;
	context->alpha_test.ref = 0;

	context->stencil_test.enabled = NMGL_FALSE;
	context->stencil_test.func = NMGL_ALWAYS;
	context->stencil_test.ref = 0;
	context->stencil_test.mask = (NMGLuint)-1;

	context->depth_test.enabled = NMGL_FALSE;
	context->depth_test.func = NMGL_LESS;
	context->depth_test.mask = NMGL_TRUE;

	context->blend.enabled = NMGL_FALSE;
	context->blend.sfactor = NMGL_ONE;
	context->blend.dfactor = NMGL_ZERO;
}

NMGL_Context *NMGL_GetCurrentContext(){
	return globalContext;
}

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
//TEXTURING_PART
SECTION(".data_imu1")	float nmgls0[NMGL_SIZE];
SECTION(".data_imu2")	float nmglt0[NMGL_SIZE];
SECTION(".data_imu3")	float nmgls1[NMGL_SIZE];
SECTION(".data_imu4")	float nmglt1[NMGL_SIZE];
SECTION(".data_imu5")	float nmgls2[NMGL_SIZE];
SECTION(".data_imu6")	float nmglt2[NMGL_SIZE];
SECTION(".data_imu7")	float nmglw0[NMGL_SIZE];
SECTION(".data_imu7")	float nmglw1[NMGL_SIZE];
SECTION(".data_imu7")	float nmglw2[NMGL_SIZE];
//TEXTURING_PART


SECTION(".data_imu5")	v4nm32f vertexResult[3 * NMGL_SIZE];
SECTION(".data_imu6")	v4nm32f colorOrNormal[3 * NMGL_SIZE];
SECTION(".data_imu6")	v2nm32f texResult[3 * NMGL_SIZE];
SECTION(".data_imu3")	v4nm32f vertexResult2[3 * NMGL_SIZE];
SECTION(".data_imu2")	v4nm32f colorOrNormal2[3 * NMGL_SIZE];
SECTION(".data_imu1")	v2nm32f texResult2[3 * NMGL_SIZE];

SECTION(".data_imu6") int masksBits[36][3 * NMGL_SIZE / 32];

#ifdef STACK_TRACE_ENABLED
SECTION(".data_imu6") StackTraceConnector stackTraceConnector;
extern "C" {
	StackTraceData nmprofiler_trace;
}
#endif //STACK_TRACE_ENABLED

extern  NMGLubyte* mipmap; //texture memory

NMGLubyte* palettes_p; // texture palette memory
unsigned int* palettes_widths_p; // texture palettes widths memory
NMGLubyte* PolygonsStipplePattern_p;
int counter = 0;



template<class T> inline T* myMallocT() {
	
	T* result = (T*)halMalloc32(sizeof32(T));
	//printf("%s, %p, sizeof32=%d\n", typeid(T).name(), result, sizeof32(T));
	if (result == 0) throw counter;
	counter++;
	return result;
}

template<class T> inline T* myMallocT(int count) {
	T* result = (T*)halMalloc32(count * sizeof32(T));
	//printf("%s, %p, sizeof32=%d\n", typeid(T).name(), result, count * sizeof32(T));
	if (result == 0) throw counter;
	counter++;
	return result;
}

SECTION(".data_imu0") NMGL_Context_NM0 *NMGL_Context_NM0::context;

#ifndef __GNUC__
#pragma optimize( "", off)
#else // __GNUC__
SECTION(".text_nmglvs")
#endif
 int nmglvsNm0Init()
{

	int fromHost = halHostSync(0xC0DE0000); // send handshake to host
	if (fromHost != 0xC0DE0086)
	{ // get  handshake from host
		return 1;
	}

	halSleep(500);

	halSetProcessorNo(0);

	NMGLSynchroData* synchroData;
	NMGL_Context_NM0 *cntxt;


	try {
		int fromNm1 = halSync(0xC0DE0000, 1);
		if (fromNm1 != 0xC0DE0001) {					// get  handshake from nm1
			return 1;
		}
		setHeap(7);
		NMGL_Context_NM0::create();
		cntxt = NMGL_Context_NM0::getContext();
		
		globalContext = myMallocT<NMGL_Context>();
		NMGL_ContextInit(globalContext);
		NMGL_Context *context = NMGL_GetCurrentContext();
		
		halSyncAddr(globalContext, 1);

		setHeap(7);
		synchroData = myMallocT<NMGLSynchroData>();
		//synchroData = new NMGLSynchroData();
		//PRINT_ADDR(synchroData);
		synchroData->init();
		cntxt->synchro.init(synchroData);
		halSyncAddr(synchroData, 1);
//TEXTURE -- PALETTE MEM INITIALIZATION
		
		setHeap(12);
		try
    	{
    	   palettes_p = (NMGLubyte*)myMallocT<NMGLubyte>(NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE*(NMGL_MAX_TEX_OBJECTS+1)); 
    	}
    	catch(int& e)
    	{
    	    printf("Error! Cant allocate texture palette memory!\n");
    	    return -1;
    	}
    	DEBUG_PRINT(("allocated palette_pointer:0x%x\n",palettes_p));
		halSyncAddr(palettes_p, 1);
		
		 try
        {
           palettes_widths_p = (unsigned int *)myMallocT<unsigned int>((NMGL_MAX_TEX_OBJECTS+1)); 
        }
        catch(int& e)
        {
            printf("Error! Cant allocate texture palette width memory!\n");
            return -1;
        }
		halSyncAddr(palettes_widths_p, 1);
		try
    	{
    	   PolygonsStipplePattern_p = (NMGLubyte*)myMallocT<NMGLubyte>(NMGL_POLIGON_STIPPLE_SIDE_UBYTES*(NMGL_POLIGON_STIPPLE_SIDE_UBYTES>>3)); 
    	}
    	catch(int& e)
    	{
    	    printf("Error! Cant allocate PolygonsStipplePattern memory!\n");
    	    return -1;
    	}
		halSyncAddr(PolygonsStipplePattern_p, 1);
		

		context->polygon.stipple.pattern = PolygonsStipplePattern_p;
		cntxt->currentSegments = &cntxt->viewportSegments;

		setHeap(10);
		PolygonsArray* trianData = myMallocT<PolygonsArray>(36);
		PolygonsArray* lineData = myMallocT<PolygonsArray>(36);
		PolygonsArray* pointsData = myMallocT<PolygonsArray>(36);

		setHeap(8);
		cntxt->triangleConnectors = myMallocT<PolygonsConnector>(36);
		cntxt->lineConnectors = myMallocT<PolygonsConnector>(36);
		cntxt->pointConnectors = myMallocT<PolygonsConnector>(36);
		//cntxt->triangleConnectors = new PolygonsConnector[36];
		//cntxt->lineConnectors = new PolygonsConnector[36];
		//cntxt->pointConnectors = new PolygonsConnector[36];
		
		for (int seg = 0; seg < 36; seg++) {
			trianData[seg].init();
			trianData[seg].ptr(0)->count = 0;
			cntxt->triangleConnectors[seg].init(trianData + seg);
			cntxt->triangleConnectors[seg].ringbufferDataPointer = trianData + seg;

			lineData[seg].init();
			lineData[seg].ptr(0)->count = 0;
			cntxt->lineConnectors[seg].init(lineData + seg);
			cntxt->lineConnectors[seg].ringbufferDataPointer = lineData + seg;

			pointsData[seg].init();
			pointsData[seg].ptr(0)->count = 0;
			cntxt->pointConnectors[seg].init(pointsData + seg);
			cntxt->pointConnectors[seg].ringbufferDataPointer = pointsData + seg;
		}

		setHeap(10);
		context->beginEndInfo.vertex = myMallocT<v4nm32f>(BIG_NMGL_SIZE);
		context->beginEndInfo.normal = myMallocT<v4nm32f>(BIG_NMGL_SIZE);
		context->beginEndInfo.color = myMallocT<v4nm32f>(BIG_NMGL_SIZE);
		//TEXTURING_PART
		context->beginEndInfo.texcoord = myMallocT<v2nm32f>(BIG_NMGL_SIZE); //XXX: Only one texture unit is supported.
		//TEXTURING_PART
		context->beginEndInfo.inBeginEnd = false;
		context->beginEndInfo.maxSize = BIG_NMGL_SIZE;

		cntxt->buffer0 = (float*)nmglBuffer0;
		cntxt->buffer1 = (float*)nmglBuffer1;
		cntxt->buffer2 = (float*)nmglBuffer2;
		cntxt->buffer3 = (float*)nmglBuffer3;
		cntxt->buffer4 = (float*)nmglBuffer4;
		cntxt->buffer5 = (float*)nmglBuffer5;
		cntxt->vertexResult = vertexResult;
		cntxt->colorOrNormal = colorOrNormal;
		cntxt->texResult = texResult;
		cntxt->vertexResult2 = vertexResult2;
		cntxt->colorOrNormal2 = colorOrNormal2;
		cntxt->texResult2 = texResult2;

		//Allocate memory for textures.
		//Must be in EMI. 
		//EMI has enough space and does not require address mapping at mc12101
		setHeap(12);
		ImageRGB8888* imagesData = myMallocT<ImageRGB8888>(2);
		DepthImage* depthImage = myMallocT<DepthImage>();
		for (int i = 0; i < WIDTH_IMAGE * HEIGHT_IMAGE; i++) {
			((int*)&imagesData[0])[i] = 0;
			((int*)&imagesData[1])[i] = 0;
			((int*)depthImage)[i] = ZBUFF_MAX;
			//((int*)depthImage)[i] = 0;
		}

		context->defaultFrameBuffer.buffers[0] = imagesData + 0;
		context->defaultFrameBuffer.buffers[1] = imagesData + 1;
		context->defaultFrameBuffer.buffers[2] = depthImage;

#ifdef TEST_NMGL_TEX_FUNC
		cntxtAddr_nm1 = (void*)halSyncAddr(0, 1);
#ifndef __NM__
		cntxtAddr_nm1 = 0; //static shared memory is not supported in x86 model
#endif //__NM__

#endif //TEST_NMGL_TEX_FUNC
		int ok = halSync(0x600DB00F, 1);
		if(ok != 0x600DB00F){
			throw 2;
		}
		//mipmap = myMallocT<NMGLubyte>(MIPMAP_MEM_SIZE); 
		//TEXTURING_PART
	}
	catch (int& e) {
		return e;
	}

	cntxt->trianInner.x0 = nmglx0;
	cntxt->trianInner.y0 = nmgly0;
	cntxt->trianInner.x1 = nmglx1;
	cntxt->trianInner.y1 = nmgly1;
	cntxt->trianInner.x2 = nmglx2;
	cntxt->trianInner.y2 = nmgly2;
	cntxt->trianInner.z = nmglz_int;
	cntxt->trianInner.s0 = nmgls0;
	cntxt->trianInner.t0 = nmglt0;
	cntxt->trianInner.s1 = nmgls1;
	cntxt->trianInner.t1 = nmglt1;
	cntxt->trianInner.s2 = nmgls2;
	cntxt->trianInner.t2 = nmglt2;
	cntxt->trianInner.w0 = nmglw0;
	cntxt->trianInner.w1 = nmglw1;
	cntxt->trianInner.w2 = nmglw2;
	cntxt->trianInner.colors = nmgllightsValues;
	cntxt->trianInner.maxSize = NMGL_SIZE;
	cntxt->trianInner.size = 0;


	cntxt->lineInner.x0 = nmglx0;
	cntxt->lineInner.y0 = nmgly0;
	cntxt->lineInner.x1 = nmglx1;
	cntxt->lineInner.y1 = nmgly1;
	cntxt->lineInner.z = nmglz_int;
	cntxt->lineInner.s0 = nmgls0;
	cntxt->lineInner.t0 = nmglt0;
	cntxt->lineInner.s1 = nmgls1;
	cntxt->lineInner.t1 = nmglt1;
	cntxt->lineInner.w0 = nmglw0;
	cntxt->lineInner.w1 = nmglw1;
	cntxt->lineInner.colors = nmgllightsValues;
	cntxt->lineInner.maxSize = NMGL_SIZE;
	cntxt->lineInner.size = 0;

	cntxt->pointInner.x = nmglx0;
	cntxt->pointInner.y = nmgly0;
	cntxt->pointInner.z = nmglz_int;
	cntxt->pointInner.s = nmgls0;
	cntxt->pointInner.t = nmglt0;
	cntxt->pointInner.w = nmglw0;
	cntxt->pointInner.colors = nmgllightsValues;
	cntxt->pointInner.maxSize = NMGL_SIZE;
	cntxt->pointInner.size = 0;
	
#ifdef __GNUC__
	halInstrCacheEnable();
	//halDmaInitC();
#endif // __GNUC__
	//sync4
	
	nmglClearColor(0, 0, 0, 1.0f);
	nmglClearDepthf(1);
	nmglViewport(0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);
	nmglScissor(0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);
	for (int i = 0; i < 36; i++) {
		cntxt->segmentMasks[i].init((nm1*)masksBits[i]);
	}
	halSync(0x600D600D, 1);

	halHostSyncAddr(&(NMGL_GetCurrentContext()->defaultFrameBuffer));
	return 0;
} 

