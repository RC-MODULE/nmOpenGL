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

#pragma code_section ".text_demo3d"

extern int exitNM1;
SECTION(".data_imu0") CommandNm1 currentCommand;
SECTION(".data_imu0") volatile int copyImageCounterColor;
SECTION(".data_imu0") volatile int copyImageCounterDepth;
SECTION(".data_shmem1") nm32s colorClearBuff[WIDTH_SEG * HEIGHT_SEG];
SECTION(".data_shmem1") nm32s depthClearBuff[WIDTH_SEG * HEIGHT_SEG];

SECTION(".text_demo3d") int copyCounterColor() {
	return copyImageCounterColor++;
	//halLed(copyImageCounterColor);
}

SECTION(".text_demo3d") int copyCounterDepth() {
	return copyImageCounterDepth++;
}

SECTION(".text_nmglvs") int nmglvsNm1Step(NMGL_Context_NM1 &cntxt)
{	

	cntxt.synchro.popInstr(&currentCommand);

	switch (currentCommand.instr_nmc1) {

	case NMC1_CLEAR: {
		int imageSize, bufferSize;
		MyDmaTask taskColor;
		MyDmaTask taskDepth;
		bufferSize = WIDTH_SEG * HEIGHT_SEG;
		taskColor.src = colorClearBuff;
		taskDepth.src = depthClearBuff;
		taskColor.size = WIDTH_SEG * HEIGHT_SEG;
		taskDepth.size = WIDTH_SEG * HEIGHT_SEG;
		taskColor.type = MSD_DMA_2D;
		taskDepth.type = MSD_DMA_2D;
		taskColor.width = WIDTH_SEG;
		taskDepth.width = WIDTH_SEG;
		taskColor.srcStride = WIDTH_SEG;
		taskDepth.srcStride = WIDTH_SEG;
		taskColor.dstStride = WIDTH_IMAGE;
		taskDepth.dstStride = WIDTH_IMAGE;
		taskColor.callback = copyCounterColor;
		taskDepth.callback = copyCounterDepth;
		copyImageCounterColor = 0;
		copyImageCounterDepth = 0;
		switch (currentCommand.params[0])
		{
		case NMGL_COLOR_BUFFER_BIT:
			imageSize = cntxt.colorBuffer.getSize();
			nmppsSet_32s((nm32s*)colorClearBuff, cntxt.colorBuffer.clearValue, bufferSize);
			for (int y = 0; y < HEIGHT_IMAGE; y += HEIGHT_SEG) {
				for (int x = 0; x < WIDTH_IMAGE; x += WIDTH_SEG) {
					taskColor.dst = (nm32s*)cntxt.colorBuffer.data + y * WIDTH_IMAGE + x;
					msdAdd(taskColor);
				}				
			}
			break;
		case NMGL_DEPTH_BUFFER_BIT:
			imageSize = cntxt.depthBuffer.getSize();
			nmppsSet_32s((nm32s*)depthClearBuff, cntxt.depthBuffer.clearValue, bufferSize);
			for (int y = 0; y < HEIGHT_IMAGE; y += HEIGHT_SEG) {
				for (int x = 0; x < WIDTH_IMAGE; x += WIDTH_SEG) {
					taskDepth.dst = (nm32s*)cntxt.depthBuffer.data + y * WIDTH_IMAGE + x;
					msdAdd(taskDepth);
				}
			}
			break;
		case NMGL_COLOR_BUFFER_BIT | NMGL_DEPTH_BUFFER_BIT:
			imageSize = cntxt.colorBuffer.getSize();
			nmppsSet_32s((nm32s*)colorClearBuff, cntxt.colorBuffer.clearValue, bufferSize);

			imageSize = cntxt.depthBuffer.getSize();
			nmppsSet_32s((nm32s*)depthClearBuff, cntxt.depthBuffer.clearValue, bufferSize);

			for (int y = 0; y < HEIGHT_IMAGE; y += HEIGHT_SEG) {
				for (int x = 0; x < WIDTH_IMAGE; x += WIDTH_SEG) {
					taskColor.dst = (nm32s*)cntxt.colorBuffer.data + y * WIDTH_IMAGE + x;
					msdAdd(taskColor);

					taskDepth.dst = (nm32s*)cntxt.depthBuffer.data + y * WIDTH_IMAGE + x;
					msdAdd(taskDepth);
				}
			}
			break;
		default:
			break;
		}
		break;
	}
	case NMC1_COPY_SEG_FROM_IMAGE:
	{		
		int x0 = currentCommand.params[0];
		int y0 = currentCommand.params[1];
		int width = currentCommand.params[2];
		int height = currentCommand.params[3];
		int numOfSeg = currentCommand.params[4];
		if (cntxt.depthBuffer.enabled == NMGL_TRUE) {
			nm32s* src = nmppsAddr_32s((int*)cntxt.depthBuffer.data, y0 * cntxt.depthBuffer.getWidth() + x0);
			nm32s* dst = (nm32s*)cntxt.smallDepthBuff.data;
			while (copyImageCounterDepth <= numOfSeg);
			msdAdd2D(src, dst, width * height, width,
				cntxt.depthBuffer.getWidth(), cntxt.smallDepthBuff.getWidth(), 1);
		}
		nm32s* src = nmppsAddr_32s((int*)cntxt.colorBuffer.data, y0 * cntxt.colorBuffer.getWidth() + x0);
		nm32s* dst = (nm32s*)cntxt.smallColorBuff.data;
		while (copyImageCounterColor <= numOfSeg);
		msdAdd2D(src, dst, width * height, width,
			cntxt.colorBuffer.getWidth(), cntxt.smallColorBuff.getWidth(), 1);
		break;
	}

	case NMC1_COPY_SEG_TO_IMAGE: {
		int x0 = currentCommand.params[0];
		int y0 = currentCommand.params[1];
		int width = currentCommand.params[2];
		int height = currentCommand.params[3];
		if (cntxt.depthBuffer.enabled == NMGL_TRUE) {
			nm32s* src = (nm32s*)cntxt.smallDepthBuff.data;
			nm32s* dst = nmppsAddr_32s((int*)cntxt.depthBuffer.data, y0 * cntxt.depthBuffer.getWidth() + x0);
			msdAdd2D(src, dst, width * height, width, 
				cntxt.smallDepthBuff.getWidth(), cntxt.depthBuffer.getWidth(), 1);
		}
		nm32s* src = (nm32s*)cntxt.smallColorBuff.data;
		nm32s* dst = nmppsAddr_32s((int*)cntxt.colorBuffer.data, y0 * cntxt.colorBuffer.getWidth() + x0);
		msdAdd2D(src, dst, width * height, width,
			cntxt.smallColorBuff.getWidth(), cntxt.colorBuffer.getWidth(), 1);
		break;
	}

	case NMC1_DRAW_TRIANGLES: {
		drawTriangles(&cntxt);
		break;
	}

	case NMC1_DRAW_LINES: {
		drawLines(&cntxt);
		break;
	}

	case NMC1_SET_COLOR: {
		int temp = currentCommand.params[2] & 0xFF;
		temp |= (currentCommand.params[1] & 0xFF) << 8;
		temp |= (currentCommand.params[0] & 0xFF) << 16;
		temp |= (currentCommand.params[3] & 0xFF) << 24;
		cntxt.colorBuffer.clearValue = temp;
		cntxt.smallColorBuff.clearValue = temp;
		break;
	}
	case NMC1_SET_DEPTH:
		cntxt.depthBuffer.clearValue = currentCommand.params[0];
		cntxt.smallDepthBuff.clearValue = currentCommand.params[0];
		break;

	case NMC1_DEPTH_MASK: {
		bool maskEnabled = currentCommand.params[0];
		cntxt.depthBuffer.setEnabledMask(maskEnabled);
		break;
	}
	case NMC1_DEPTH_FUNC: {
		int depthMode = currentCommand.params[0];
		cntxt.depthBuffer.setMode(depthMode);
		break;
	}
	case NMC1_SWAP_BUFFER: {

		msdWaitDma();
		
		cntxt.t1 = clock();
		cntxt.synchro.counter++;
		cntxt.synchro.time = cntxt.t1 - cntxt.t0;
		cntxt.imagesData->head++;
		cntxt.colorBuffer.data = cntxt.imagesData->ptrHead();
		while (cntxt.imagesData->isFull());
		cntxt.t0 = clock();

		break;
	}
	case NMC1_EXIT:
		exitNM1 = 1;
		break;

	case NMC1_DEPTH:
		cntxt.depthBuffer.enabled = currentCommand.params[0];
		break;
	case NMC1_AND: {
		int* src0 = (int*)currentCommand.params[0];
		int* src1 = (int*)currentCommand.params[1];
		nm32u* dst = (nm32u*)currentCommand.params[2];
		int size = currentCommand.params[3];
		nmppsCopy_32s(src0, cntxt.buffer0, size);
		nmppsCopy_32s(src1, cntxt.buffer1, size);
		nmppsAnd_32u((nm32u*)cntxt.buffer0, (nm32u*)cntxt.buffer1, dst, size);
		break;
	}
	case NMC1_AND4: {
		int* src0Ext = (int*)currentCommand.params[0];
		int* src1Ext = (int*)currentCommand.params[1];
		int* src2Ext = (int*)currentCommand.params[2];
		int* src3Ext = (int*)currentCommand.params[3];
		nm64u* dst = (nm64u*)currentCommand.params[4];
		
		int size = currentCommand.params[5];
		while (size > 0) {
			int localSize = (size, SIZE_BANK / 2);
			nm64u* src0 = (nm64u*)(cntxt.buffer0);
			nm64u* src1 = (nm64u*)(cntxt.buffer1);
			nm64u* src2 = (nm64u*)(cntxt.buffer0 + SIZE_BANK / 2);
			nm64u* src3 = (nm64u*)(cntxt.buffer1 + SIZE_BANK / 2);
			nmppsCopy_32s(src0Ext, (nm32s*)src0, localSize);
			nmppsCopy_32s(src1Ext, (nm32s*)src1, localSize);
			nmppsCopy_32s(src2Ext, (nm32s*)src2, localSize);
			nmppsCopy_32s(src3Ext, (nm32s*)src3, localSize);
			nmppsAnd4V_64u(src0, src1, src2, src3, dst, localSize / 2);
			size -= localSize;
			src0Ext += localSize;
			src1Ext += localSize;
			src2Ext += localSize;
			src3Ext += localSize;
			dst += localSize / 2;
		}
		break;
	}
	case NMC1_OR: {
		int* src1 = (int*)currentCommand.params[0];
		int* src2 = (int*)currentCommand.params[1];
		nm32u* dst = (nm32u*)currentCommand.params[2];
		int size = currentCommand.params[3];
		nmppsCopy_32s(src1, cntxt.buffer0, size);
		nmppsCopy_32s(src2, cntxt.buffer1, size);
		nmppsOr_32u((nm32u*)cntxt.buffer0, (nm32u*)cntxt.buffer1, dst, size);
		break;
	}
	case NMC1_CNV_32S_8S: {
		int* src = (int*)currentCommand.params[0];
		nm8s* dst = (nm8s*)currentCommand.params[1];
		int size = currentCommand.params[2];
		nmppsCopy_32s(src, cntxt.buffer0, size);
		nmppsConvert_32s8s((nm32s*)cntxt.buffer0, dst, size);
		break;
	}
	
	case NMC1_SET_ACTIVE_TEXTURE: {
		cntxt.texState.activeTexUnitIndex = currentCommand.params[0];
		break;
	}

	default:
		break;
	}
	//printf("synchro: head-tail=%d\n", cntxt.synchro->commandsRB.head - cntxt.synchro->commandsRB.tail);
	//printf("poly: head-tail=%d\n", cntxt.polygonsRB->head - cntxt.polygonsRB->tail);
	//printf("image: head-tail=%d\n\n", cntxt.colorBuffer->ringbuffer.head - cntxt.colorBuffer->ringbuffer.tail);
	return 0;
} 

