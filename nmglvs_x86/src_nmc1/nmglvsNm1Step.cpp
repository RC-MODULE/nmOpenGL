#include "hal_target.h"
#include "hal.h"
#include "hal.h"
#include "hal_target.h"
#include "nmpp.h"
#include "demo3d_nm1.h"
#include "stdio.h"
#include "ringbuffer.h"
#include "nmprofiler.h"
#include "cache.h"

#include "nmgl.h"
#include "nmgl_data1.h"

#define START_TIME t0 = clock();
#define END_TIME t1 = clock(); time_nmc1 += t1 - t0;

#pragma code_section ".text_demo3d"

unsigned time_nmc1 = 0;
unsigned counter_measure_time1 = 0;

extern int exitNM1;

int nmglvsNm1Step()
{
	clock_t t0, t1;
	START_TIME;
	CHECK_STACK_COMMAND;
	END_TIME;

	switch (COMMAND) {

	case NMC1_CLEAR_COLOR:
	{
		START_TIME;
		while (halRingBufferIsFull(&imageRB));
		int size = nmglSynchro->widthWindow * nmglSynchro->heightWindow;
		initSetArray((nm32s*)IMAGE_BUFFER, clearColorValue, size);
		END_TIME;
	}
	break;

	case NMC1_CLEAR_DEPTH:
	{
		START_TIME;
		int size = nmglSynchro->widthWindow * nmglSynchro->heightWindow;
		initSetArray(ZBuffImage, clearDepthValue, size);
		END_TIME;
		break;
	}
	case NMC1_INIT_COPY_SEG_FROM_IMAGE:
	{
		int x0 = READ_PARAM0_NMC1;
		int y0 = READ_PARAM1_NMC1;
		int width = READ_PARAM2_NMC1;
		int height = READ_PARAM3_NMC1;

		START_TIME;
		while (halRingBufferIsFull(&imageRB));
		END_TIME;

		nm32s* dstImage = nmppsAddr_32s((nm32s*)IMAGE_BUFFER, y0 * nmglSynchro->widthWindow + x0);
		nm32s* dstZ = nmppsAddr_32s((nm32s*)ZBuffImage, y0 * nmglSynchro->widthWindow + x0);

		setDmaStatus();
		if (isDepthTest == NMGL_FALSE) {
			halSetCallbackDMA((DmaCallback)cbDmaReady);
			halDma2D_StartA(dstImage, segImage, width * height, width, nmglSynchro->widthWindow, WIDTH_SEG);
		}
		else {
			halSetCallbackDMA((DmaCallback)cbCopySegFromImage);
			setSegForDma(dstImage, segImage, width, height);
			halDma2D_StartA(dstZ, segZBuff, width * height, width, nmglSynchro->widthWindow, WIDTH_SEG);
		}
		while (dmaIsBusy());
		break;
	}

	case NMC1_INIT_COPY_SEG_TO_IMAGE: {
		int x0 = READ_PARAM0_NMC1;
		int y0 = READ_PARAM1_NMC1;
		int width = READ_PARAM2_NMC1;
		int height = READ_PARAM3_NMC1;

		nm32s* dstImage = nmppsAddr_32s((nm32s*)IMAGE_BUFFER, y0 * nmglSynchro->widthWindow + x0);
		nm32s* dstZ = nmppsAddr_32s((nm32s*)ZBuffImage, y0 * nmglSynchro->widthWindow + x0);

		setDmaStatus();
		if (isDepthTest == NMGL_FALSE) {
			halSetCallbackDMA((DmaCallback)cbDmaReady);
			halDma2D_StartA(segImage, dstImage, width * height, width, WIDTH_SEG, nmglSynchro->widthWindow);
		}
		else {
			halSetCallbackDMA((DmaCallback)cbCopySegToImage);
			setSegForDma(segImage, dstImage, width, height);
			halDma2D_StartA(segZBuff, dstZ, width * height, width, WIDTH_SEG, nmglSynchro->widthWindow);
		}
		break;
	}

	case NMC1_DRAW_TRIANGLES: {
		drawTriangles();
		break;
	}

	case NMC1_DRAW_LINES: {
		drawLines();
		break;
	}


	case NMC1_SET_COLOR:
		clearColorValue = READ_PARAM2_NMC1 & 0xFF;
		clearColorValue |= (READ_PARAM1_NMC1 & 0xFF) << 8;
		clearColorValue |= (READ_PARAM0_NMC1 & 0xFF) << 16;
		clearColorValue |= (READ_PARAM3_NMC1 & 0xFF) << 24;
		break;

	case NMC1_SET_DEPTH:
		clearColorValue = READ_PARAM0_NMC1 & 0xFF;
		break;

	case NMC1_DEPTH_MASK:
		if (READ_PARAM0_NMC1 == NMGL_TRUE) {
			depthFunc = depthFunc_rw;
		}
		else {
			depthFunc = depthFunc_r;
		}
		break;
	case NMC1_DEPTH_FUNC:
		break;

	case NMC1_SWAP_BUFFER: {
		START_TIME;
		while (dmaIsBusy() || halRingBufferIsFull(&imageRB));
		END_TIME;
	
		/*v4nm8s* src;
		nm8s* dst;
		halSetCallbackDMA((DmaCallback)cbDmaReady);
		for(int k = 0; k < WIDTH_IMAGE * HEIGHT_IMAGE; k+= SIZE_BANK){
			src = (v4nm8s*)nmppsAddr_32s(IMAGE_BUFFER, k);
			dst = (nm8s*)nmppsAddr_8s((nm8s*)halRingBufferHead(&imageRB), k);
			setDmaStatus();
			halDmaStartA(src, pool0, SIZE_BANK);
			while(dmaIsBusy());
			selectColorChannel((v4nm8s*)pool0, 0, (nm8s*)dst, SIZE_BANK);
		}*/

		imageRB.head++;
		nmglSynchro->counter_nmc1++;
		nmglSynchro->time1 = time_nmc1;
		time_nmc1 = 0;
		counter_measure_time1 = 0;

		break;
	}
	case NMC1_EXIT:
		exitNM1 = 1;
		break;
	case NMC1_DEPTH:
		isDepthTest = READ_PARAM0_NMC1;
		break;
	default:
		break;
	}
	END_INSTR
		return 0;
} 

