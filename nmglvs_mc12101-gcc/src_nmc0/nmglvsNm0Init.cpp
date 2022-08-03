#include "hal_target.h"
#include "hal.h"
//#include <time.h>
#include <nmpp.h>
#include "demo3d_nm0.h"
#include "cache.h"
#include "ringbuffer.h"
#include "context_float.h"
#include "debugprint.h"
#include "nmprofiler.h"
#include "nmglremote.h"

#ifdef STACK_TRACE_ENABLED
#include "stacktrace.h"
#endif //STACK_TRACE_ENABLED

#include "nmgl.h"

#define PRINT_ADDR(a) printf(#a "=%p\n", a)
#define PRINT_HEX(a) printf(#a "=0x%x\n", a)

#define CHECK_EXIT0 if(nmglSynchro->exit_nm==EXIT) {	break;	}
#ifdef TEST_NMGL_TEX_FUNC
SECTION(".data_imu0") void* cntxtAddr_nm1;
#endif

SECTION(".data_imu2")	float nmglx0[NMGL_SIZE];
SECTION(".data_imu2")	float nmgly0[NMGL_SIZE];
SECTION(".data_imu2")	float nmglx1[NMGL_SIZE];
SECTION(".data_imu3")	float nmgly1[NMGL_SIZE];
SECTION(".data_imu3")	float nmglx2[NMGL_SIZE];
SECTION(".data_imu3")	float nmgly2[NMGL_SIZE];
SECTION(".data_imu4")	int nmglz_int[NMGL_SIZE];
//SECTION(".data_imu6")	v4nm32s nmgllightsValues[NMGL_SIZE];
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
SECTION(".data_imu6")	v4nm32f normalResult[3 * NMGL_SIZE];
SECTION(".data_imu2")	v4nm32f colorResult[3 * NMGL_SIZE];
SECTION(".data_imu6")	v2nm32f texResult[3 * NMGL_SIZE];
SECTION(".data_imu3")	v4nm32f vertexResult2[3 * NMGL_SIZE];
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

extern int nmprofiler_head_addr;

SECTION(".text_nmglvs") int nmglvsNm0Init()
{
	//halSleep(500);

	halSetProcessorNo(0);

	NMGLSynchroData* synchroData;
	NMGL_Context_NM0 *cntxt;
	ImageData* imagesData;
	NMGL_RemoteAccess *remoteAccess = NMGL_RemoteAccessInstance();

	DEBUG_PLOG_LEVEL_0("handshake\n");
	int fromHost = halHostSync(0xC0DE0000);		// send handshake to host
	if (fromHost != 0xC0DE0086) {					// get  handshake from host
		DEBUG_PLOG_ERROR("handshake fail\n");
		return 1;
	}
	DEBUG_PLOG_LEVEL_0("Handshake OK\n");
	halHostSyncAddr(remoteAccess);		// send handshake to host


	try {
		
		int fromNm1 = halSync(0xC0DE0000, 1);
		if (fromNm1 != 0xC0DE0001) {					// get  handshake from nm1
			DEBUG_PLOG_ERROR("Interprocessor sync fail!!\n");
			return 1;
		}
		DEBUG_PLOG_LEVEL_0("Interprocessor sync OK\n");
		setHeap(7);

		synchroData = myMallocT<NMGLSynchroData>();
		//synchroData = new NMGLSynchroData();
		//PRINT_ADDR(synchroData);
		synchroData->init();
		halSyncAddr(synchroData, 1);
//TEXTURE -- PALETTE MEM INITIALIZATION
		
		setHeap(12);
		try
    	{
    	   palettes_p = (NMGLubyte*)myMallocT<NMGLubyte>(NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE*(NMGL_MAX_TEX_OBJECTS+1)); 
    	}
    	catch(int& e)
    	{
    	    DEBUG_PLOG_LEVEL_0("Error! Cant allocate texture palette memory!\n");
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
            DEBUG_PLOG_LEVEL_0("Error! Cant allocate texture palette width memory!\n");
            return -1;
        }
		halSyncAddr(palettes_widths_p, 1);
		try
    	{
    	   PolygonsStipplePattern_p = (NMGLubyte*)myMallocT<NMGLubyte>(NMGL_POLIGON_STIPPLE_SIDE_UBYTES*(NMGL_POLIGON_STIPPLE_SIDE_UBYTES>>3)); 
    	}
    	catch(int& e)
    	{
    	    DEBUG_PLOG_LEVEL_0("Error! Cant allocate PolygonsStipplePattern memory!\n");
    	    return -1;
    	}
		halSyncAddr(PolygonsStipplePattern_p, 1);

		remoteAccess->initializingProcess = 0.3f;

		setHeap(7);
		NMGL_Context_NM0::create();
		cntxt = NMGL_Context_NM0::getContext();
		remoteAccess->framebuffer = &cntxt->defaultFramebuffer;
		printf("remoteAccess->framebuffer=0x%x, offset=%d\n", remoteAccess->framebuffer);
		cntxt->synchro.init(synchroData);		

		cntxt->polygon.stipple.pattern = PolygonsStipplePattern_p;

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
		cntxt->beginEndInfo.vertex = myMallocT<v4nm32f>(BIG_NMGL_SIZE);
		cntxt->beginEndInfo.normal = myMallocT<v4nm32f>(BIG_NMGL_SIZE);
		cntxt->beginEndInfo.color = myMallocT<v4nm32f>(BIG_NMGL_SIZE);
		//TEXTURING_PART
		cntxt->beginEndInfo.texcoord = myMallocT<v2nm32f>(BIG_NMGL_SIZE); //XXX: Only one texture unit is supported.
		//TEXTURING_PART
		cntxt->beginEndInfo.inBeginEnd = false;
		cntxt->beginEndInfo.maxSize = BIG_NMGL_SIZE;

		auto core_context = getCoreContextFloat();
		cntxt->buffer0 = core_context->pools[0].f;
		cntxt->buffer1 = core_context->pools[1].f;
		cntxt->buffer2 = core_context->pools[2].f;
		cntxt->buffer3 = core_context->pools[3].f;
		cntxt->buffer4 = core_context->pools[4].f;
		cntxt->buffer5 = core_context->pools[5].f;
		cntxt->vertexResult = vertexResult;
		cntxt->colorResult = colorResult;
		cntxt->normalResult = normalResult;
		cntxt->texResult = texResult;
		cntxt->vertexResult2 = vertexResult2;
		cntxt->colorResult2 = normalResult;
		cntxt->normalResult2 = colorResult;
		cntxt->texResult2 = texResult2;

		remoteAccess->initializingProcess = 0.5f;

		//Allocate memory for textures.
		//Must be in EMI. 
		//EMI has enough space and does not require address mapping at mc12101
		setHeap(12);
		NMGL_FramebufferInit(&cntxt->defaultFramebuffer, WIDTH_IMAGE, HEIGHT_IMAGE);
		
		ImageRGB8888* imagesData0 = myMallocT<ImageRGB8888>();
		ImageRGB8888* imagesData1 = myMallocT<ImageRGB8888>();
		DepthImage* depthImage = myMallocT<DepthImage>();
		DEBUG_PLOG_LEVEL_0("Alloc image OK!\n");

		for (int i = 0; i < WIDTH_IMAGE * HEIGHT_IMAGE; i++) {
			((int*)imagesData0)[i] = 0;
			((int*)imagesData1)[i] = 0;
			((int*)depthImage)[i] = ZBUFF_MAX;
			//((int*)depthImage)[i] = 0;
		}

		cntxt->defaultFramebuffer.buffers[0] = imagesData0;
		cntxt->defaultFramebuffer.buffers[1] = imagesData1;
		cntxt->defaultFramebuffer.buffers[2] = depthImage;

		halSyncAddr(&cntxt->defaultFramebuffer, 1);
		DEBUG_PLOG_LEVEL_0("Share image with nmc1 OK!\n");
		remoteAccess->initializingProcess = 0.7f;

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
		mipmap = myMallocT<NMGLubyte>(MIPMAP_MEM_SIZE); 
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
	//cntxt->trianInner.colors = nmgllightsValues;
	cntxt->trianInner.colors = (v4nm32s*)normalResult;
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
	//cntxt->lineInner.colors = nmgllightsValues;
	cntxt->lineInner.colors = (v4nm32s*)normalResult;
	cntxt->lineInner.maxSize = NMGL_SIZE;
	cntxt->lineInner.size = 0;

	cntxt->pointInner.x = nmglx0;
	cntxt->pointInner.y = nmgly0;
	cntxt->pointInner.z = nmglz_int;
	cntxt->pointInner.s = nmgls0;
	cntxt->pointInner.t = nmglt0;
	cntxt->pointInner.w = nmglw0;
	//cntxt->pointInner.colors = nmgllightsValues;
	cntxt->pointInner.colors = (v4nm32s*)normalResult;
	cntxt->pointInner.maxSize = NMGL_SIZE;
	cntxt->pointInner.size = 0;
	
#ifdef __GNUC__
	halInstrCacheEnable();
	//halDmaInitC();
#endif // __GNUC__
	//sync4
	remoteAccess->initializingProcess = 0.9f;
	nmglClearColor(0, 0, 0, 1.0f);
	nmglClearDepthf(1);
	nmglViewport(0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);
	cntxt->windowInfo.imageSize.width = WIDTH_IMAGE;
	cntxt->windowInfo.imageSize.height = HEIGHT_IMAGE;
	nmglScissor(0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);
	nmglDisable(NMGL_SCISSOR_TEST);
	for (int i = 0; i < 36; i++) {
		cntxt->segmentMasks[i].init((nm1*)masksBits[i]);
	}
	halSync(0x600D600D, 1);


	remoteAccess->initializingProcess = 1;
	DEBUG_PLOG_LEVEL_0("nmOpenGL inited!\n");
	/*int profiler_enabled = halHostSyncAddr(remoteAccess->framebuffer);
	DEBUG_PLOG_LEVEL_0("Send addr of framebuffer\n");
#ifdef __NM__
	if(profiler_enabled){
		nmprofiler_init();
		halHostSync(nmprofiler_head_addr);
		//nmprofiler_disabled();
	}
#endif*/
	NMGL_RemoteAccessProcessEvents();
	
	return 0;
} 

