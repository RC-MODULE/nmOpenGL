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

SECTION(".text_nmglvs") int nmglvsNm1Step(NMGL_Context_NM1 &cntxt)
{	

	cntxt.synchro.popInstr(&currentCommand);

	switch (currentCommand.instr_nmc1) {

	case NMC1_CLEAR:
		int imageSize, bufferSize;

		switch (currentCommand.params[0])
		{
		case NMGL_COLOR_BUFFER_BIT:
			imageSize = cntxt.colorBuffer.getSize();
			bufferSize = cntxt.colorSegment.getSize();
			nmppsSet_32s((nm32s*)cntxt.colorSegment.data, cntxt.colorBuffer.clearValue, bufferSize);
			for (int i = 0; i < imageSize; i += bufferSize) {
				msdAdd(cntxt.colorSegment.data, (nm32s*)cntxt.colorBuffer.data + i, bufferSize);
			}
			break;
		case NMGL_DEPTH_BUFFER_BIT:
			imageSize = cntxt.depthBuffer.getSize();
			bufferSize = cntxt.depthSegment.getSize();
			nmppsSet_32s((nm32s*)cntxt.depthSegment.data, cntxt.depthBuffer.clearValue, bufferSize);
			for (int i = 0; i < imageSize; i += bufferSize) {
				msdAdd(cntxt.depthSegment.data, (nm32s*)cntxt.depthBuffer.data + i, bufferSize);
			}
			break;
		case NMGL_COLOR_BUFFER_BIT | NMGL_DEPTH_BUFFER_BIT:
			imageSize = cntxt.colorBuffer.getSize();
			bufferSize = cntxt.colorSegment.getSize();
			nmppsSet_32s((nm32s*)cntxt.colorSegment.data, cntxt.colorBuffer.clearValue, bufferSize);
			for (int i = 0; i < imageSize; i += bufferSize) {
				msdAdd(cntxt.colorSegment.data, (nm32s*)cntxt.colorBuffer.data + i, bufferSize);
			}
			imageSize = cntxt.depthBuffer.getSize();
			bufferSize = cntxt.depthSegment.getSize();
			nmppsSet_32s((nm32s*)cntxt.depthSegment.data, cntxt.depthBuffer.clearValue, bufferSize);
			for (int i = 0; i < imageSize; i += bufferSize) {
				msdAdd(cntxt.depthSegment.data, (nm32s*)cntxt.depthBuffer.data + i, bufferSize);
			}
			break;
		default:
			break;
		}		
		break;

	case NMC1_COPY_SEG_FROM_IMAGE:
	{		
		int x0 = currentCommand.params[0];
		int y0 = currentCommand.params[1];
		int width = currentCommand.params[2];
		int height = currentCommand.params[3];
		if (cntxt.depthBuffer.enabled == NMGL_TRUE) {
			nm32s* src = nmppsAddr_32s((int*)cntxt.depthBuffer.data, y0 * cntxt.depthBuffer.getWidth() + x0);
			nm32s* dst = (nm32s*)cntxt.depthSegment.data;
			msdAdd2D(src, dst, width * height, width, 
				cntxt.depthBuffer.getWidth(), cntxt.depthSegment.getWidth());
		}
		nm32s* src = nmppsAddr_32s((int*)cntxt.colorBuffer.data, y0 * cntxt.colorBuffer.getWidth() + x0);
		nm32s* dst = (nm32s*)cntxt.colorSegment.data;
		msdAdd2D(src, dst, width * height, width, 
			cntxt.colorBuffer.getWidth(), cntxt.colorSegment.getWidth());
		break;
	}

	case NMC1_COPY_SEG_TO_IMAGE: {
		int x0 = currentCommand.params[0];
		int y0 = currentCommand.params[1];
		int width = currentCommand.params[2];
		int height = currentCommand.params[3];
		if (cntxt.depthBuffer.enabled == NMGL_TRUE) {
			nm32s* src = (nm32s*)cntxt.depthSegment.data;
			nm32s* dst = nmppsAddr_32s((int*)cntxt.depthBuffer.data, y0 * cntxt.depthBuffer.getWidth() + x0);
			msdAdd2D(src, dst, width * height, width, 
				cntxt.depthSegment.getWidth(), cntxt.depthBuffer.getWidth());
		}
		nm32s* src = (nm32s*)cntxt.colorSegment.data;
		nm32s* dst = nmppsAddr_32s((int*)cntxt.colorBuffer.data, y0 * cntxt.colorBuffer.getWidth() + x0);
		msdAdd2D(src, dst, width * height, width,
			cntxt.colorSegment.getWidth(), cntxt.colorBuffer.getWidth());
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
		break;
	}
	case NMC1_SET_DEPTH:
		cntxt.depthBuffer.clearValue = currentCommand.params[0];
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

	default:
		break;
	}
	//printf("synchro: head-tail=%d\n", cntxt.synchro->commandsRB.head - cntxt.synchro->commandsRB.tail);
	//printf("poly: head-tail=%d\n", cntxt.polygonsRB->head - cntxt.polygonsRB->tail);
	//printf("image: head-tail=%d\n\n", cntxt.colorBuffer->ringbuffer.head - cntxt.colorBuffer->ringbuffer.tail);
	return 0;
} 

