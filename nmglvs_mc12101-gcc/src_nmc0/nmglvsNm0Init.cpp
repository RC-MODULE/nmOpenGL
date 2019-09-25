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
#define STACK_COMMAND_SIZE 256
#define INSTANT_STACK_COMMAND_SIZE 4

SECTION(".data_shared")	Synchro s_synchro;
SECTION(".data_shmem0")	HalRingBuffer s_nmglPolygonsRB;
SECTION(".data_shared")	Polygons polyArray[COUNT_POLYGONS_BUFFER];
SECTION(".data_shmem0")	CommandNm1 commands[STACK_COMMAND_SIZE];
SECTION(".data_shmem0")	CommandNm1 instantCommands[INSTANT_STACK_COMMAND_SIZE];

SECTION(".data_imu1")	float nmglBuffer0[12 * NMGL_SIZE];
SECTION(".data_imu2")	float nmglBuffer1[12 * NMGL_SIZE];
SECTION(".data_imu3")	float nmglBuffer2[12 * NMGL_SIZE];
SECTION(".data_imu4")	float nmglBuffer3[12 * NMGL_SIZE];


void synchroInit(Synchro* nmglSynchro, int widthImage, int heightImage) {
	nmglSynchro->exit_nm = 0;
	nmglSynchro->counter_nmc0 = 0;
	nmglSynchro->counter_nmc1 = 0;
	nmglSynchro->hasInstrHost = 0;
	nmglSynchro->time0 = 0;
	nmglSynchro->time1 = 0;
	halRingBufferInit(&nmglSynchro->commandsRB, commands, sizeof32(CommandNm1), STACK_COMMAND_SIZE, 0, 0, 0);
	halRingBufferInit(&nmglSynchro->instantCommandsRB, instantCommands, sizeof32(CommandNm1), INSTANT_STACK_COMMAND_SIZE, 0, 0, 0);
}

#pragma code_section ".text_demo3d"
int nmglvsNm0Init()
{
	halSetProcessorNo(0);
	int fromHost = halHostSync(0xC0DE0000);		// send handshake to host
	if (fromHost != 0xC0DE0086) {					// get  handshake from host
		return -1;
	}

	halInstrCacheEnable();
	//nmprofiler_init();

	cntxt.buffer0 = nmglBuffer0;
	cntxt.buffer1 = nmglBuffer1;
	cntxt.buffer2 = nmglBuffer2;
	cntxt.buffer3 = nmglBuffer3;

	//matrices
	cntxt.modelviewMatrixStack.base = cntxt.modelviewMatrix;
	cntxt.modelviewMatrixStack.size = 16;
	cntxt.modelviewMatrixStack.current = 0;
	cntxt.modelviewMatrixStack.type = NMGL_MODELVIEW_MATRIX;

	cntxt.projectionMatrixStack.base = cntxt.projectionMatrix;
	cntxt.projectionMatrixStack.size = 2;
	cntxt.projectionMatrixStack.current = 0;
	cntxt.projectionMatrixStack.type = NMGL_PROJECTION_MATRIX;

	cntxt.currentMatrixStack = &cntxt.modelviewMatrixStack;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			cntxt.modelviewMatrix[0].matr[j * 4 + i] = (i == j) ? 1 : 0;
			cntxt.projectionMatrix[0].matr[j * 4 + i] = (i == j) ? 1 : 0;
		}
	}

	//light
	cntxt.isLighting = NMGL_FALSE;
	//tmp = {0,0,0,1}
	cntxt.tmp.vec[0] = cntxt.tmp.vec[1] = cntxt.tmp.vec[2] = 0;
	cntxt.tmp.vec[3] = 1;
	set_v4nm32f(cntxt.lightAmbient, &cntxt.tmp, MAX_LIGHTS + 1);
	set_v4nm32f(cntxt.lightDiffuse, &cntxt.tmp, MAX_LIGHTS);
	cntxt.lightDiffuse[0].vec[0] = cntxt.lightDiffuse[0].vec[1] = cntxt.lightDiffuse[0].vec[2] = 1;
	set_v4nm32f(cntxt.lightSpecular, &cntxt.tmp, MAX_LIGHTS);
	cntxt.lightSpecular[0].vec[0] = cntxt.lightSpecular[0].vec[1] = cntxt.lightSpecular[0].vec[2] = 1;
	//tmp = {0,0,1,0}
	cntxt.tmp.vec[2] = 1;
	cntxt.tmp.vec[3] = 0;
	set_v4nm32f(cntxt.lightPosition, &cntxt.tmp, MAX_LIGHTS);
	//tmp = {0,0,-1,0}
	cntxt.tmp.vec[2] = -1;
	set_v4nm32f(cntxt.lightSpotDirection, &cntxt.tmp, MAX_LIGHTS);
	for (int i = 0; i < MAX_LIGHTS; i++) {
		cntxt.lightSpotExp[i] = 0;
		cntxt.lightSpotCutoff[i] = 180;
		cntxt.lightConstAtt[i] = 1;
		cntxt.lightLinAtt[i] = 0;
		cntxt.lightQuadAtt[i] = 0;
		cntxt.isEnabledLight[i] = 0;
	}
	for (int i = 0; i < 3; i++) {
		cntxt.materialAmbient.vec[i] = 0.2;
		cntxt.materialDiffuse.vec[i] = 0.8;
		cntxt.materialSpecular.vec[i] = 0;
		cntxt.materialEmissive.vec[i] = 0;
	}
	cntxt.materialAmbient.vec[3] = 1;
	cntxt.materialDiffuse.vec[3] = 1;
	cntxt.materialSpecular.vec[3] = 1;
	cntxt.materialEmissive.vec[3] = 1;
	cntxt.specularExp = 0;
	cntxt.isUseTwoSidedMode = NMGL_FALSE;

	cntxt.windowInfo.segmentWidth = WIDTH_SEG;
	cntxt.windowInfo.segmentHeight = HEIGHT_SEG;
	cntxt.windowInfo.viewportMulZ = (1 - 0) * 0.5 * ZBUFF_MAX;
	cntxt.windowInfo.viewportAddZ = (1 + 0) * 0.5 * ZBUFF_MAX;

	//массивы
	nmglDisableClientState(NMGL_VERTEX_ARRAY);
	nmglDisableClientState(NMGL_COLOR_ARRAY);
	nmglDisableClientState(NMGL_NORMAL_ARRAY);

	//
	cntxt.isCullFace = NMGL_FALSE;
	cntxt.cullFaceType = NMGL_BACK;
	cntxt.frontFaceOrientation = NMGL_CCW;
	//
	cntxt.synchro = &s_synchro;
	nmglPolygonsRB = &s_nmglPolygonsRB;
	//Структура для общения процессорных ядер
	synchroInit(cntxt.synchro, WIDTH_IMAGE, HEIGHT_IMAGE);

	//Массив Polygons-структур
	halSyncAddr((int*)cntxt.synchro, 1);
	halRingBufferInit(nmglPolygonsRB, polyArray, sizeof32(Polygons), COUNT_POLYGONS_BUFFER, 0, 0, 0);

	//Адрес кольцевого буфера Polygons-структур на nmc1
	halSyncAddr((int*)nmglPolygonsRB, 1);

	//halDmaInitC();
	// Check memory allocation
	if (cntxt.synchro == 0) {
		halHostSync(0xDEADB00F);	// send error to host
		return -1;
	}
	else {
		halHostSync(0x600DB00F);	// send ok to host
	}


	//sync3
	halHostSync((int)cntxt.synchro);

	//sync4
	halHostSync((int)0x600d600d);
	halDmaInit();

	nmglViewport(0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);

} 

