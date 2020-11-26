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

SECTION(".text_demo3d") int copyCounterColor() {
	return copyImageCounterColor++;
}

SECTION(".text_demo3d") int copyCounterDepth() {
	return copyImageCounterDepth++;
}




SECTION(".text_demo3d") int swapImage() {
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	cntxt->t1 = clock();
	//cntxt->synchro.counter++;
	//cntxt->synchro.time = cntxt->t1 - cntxt->t0;
	cntxt->imagesData->head++;
	cntxt->colorBuffer.mData = cntxt->imagesData->ptrHead();
	while (cntxt->imagesData->isFull());
	cntxt->t0 = clock();
	return 0;
}

SECTION(".text_nmglvs") int nmglvsNm1Step()
{	
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	cntxt->synchro.popInstr(&currentCommand);

	switch (currentCommand.instr_nmc1) {

	case NMC1_CLEAR: {
		msdWaitDma();
		cntxt->smallClearColorBuff.setCursor(0, 0);
		cntxt->smallClearDepthBuff.setCursor(0, 0);
		int widthImage = cntxt->colorBuffer.getWidth();
		int heightImage = cntxt->colorBuffer.getHeight();
		int widthSeg = cntxt->smallClearColorBuff.getWidth();
		int heightSeg = cntxt->smallClearColorBuff.getHeight();
		copyImageCounterColor = 0;
		copyImageCounterDepth = 0;
		switch (currentCommand.params[0])
		{
		case NMGL_COLOR_BUFFER_BIT:
			cntxt->smallClearColorBuff.clear();
			for (int y = 0; y < heightImage; y += heightSeg) {
				for (int x = 0; x < widthImage; x += widthSeg) {
					cntxt->colorBuffer.setCursor(x, y);
					int width = MIN(widthSeg, widthImage - x);
					int height = MIN(heightSeg, heightImage - y);
					PROFILER_SIZE(width * height);
					cntxt->smallClearColorBuff.pushWindow(cntxt->colorBuffer, width, height, copyCounterColor);
				}
			}
			break;
		case NMGL_DEPTH_BUFFER_BIT:
			cntxt->smallClearDepthBuff.clear();
			for (int y = 0; y < heightImage; y += heightSeg) {
				for (int x = 0; x < widthImage; x += widthSeg) {
					cntxt->depthBuffer.setCursor(x, y);
					int width = MIN(widthSeg, widthImage - x);
					int height = MIN(heightSeg, heightImage - y);
					PROFILER_SIZE(width * height);
					cntxt->smallClearDepthBuff.pushWindow(cntxt->depthBuffer, width, height, copyCounterDepth);
				}
			}
			break;
		case NMGL_COLOR_BUFFER_BIT | NMGL_DEPTH_BUFFER_BIT:
			cntxt->smallClearColorBuff.clear();
			cntxt->smallClearDepthBuff.clear();
			for (int y = 0; y < heightImage; y += heightSeg) {
				for (int x = 0; x < widthImage; x += widthSeg) {
					cntxt->colorBuffer.setCursor(x, y);
					cntxt->depthBuffer.setCursor(x, y);
					int width = MIN(widthSeg, widthImage - x);
					int height = MIN(heightSeg, heightImage - y);
					PROFILER_SIZE(width * height);
					cntxt->smallClearColorBuff.pushWindow(cntxt->colorBuffer, width, height, copyCounterColor);
					PROFILER_SIZE(width * height);
					cntxt->smallClearDepthBuff.pushWindow(cntxt->depthBuffer, width, height, copyCounterDepth);
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
		//printf("x0=%d, y=%d, w=%d, h=%d, i=%d\n",x0, y0, width, height, numOfSeg);
		//msdWaitDma(1);
		if (cntxt->depthBuffer.enabled == NMGL_TRUE) {
			cntxt->depthBuffer.setCursor(x0, y0);
			cntxt->smallDepthBuff.setCursor(0, 0);
			while (copyImageCounterDepth <= numOfSeg);
			PROFILER_SIZE(width * height);
			cntxt->depthBuffer.pushWindow(cntxt->smallDepthBuff, width, height);
		}
		cntxt->colorBuffer.setCursor(x0, y0);
		cntxt->smallColorBuff.setCursor(0, 0);
		while (copyImageCounterColor <= numOfSeg);
		PROFILER_SIZE(width * height);
		cntxt->colorBuffer.pushWindow(cntxt->smallColorBuff, width, height);
		break;
	}

	case NMC1_COPY_SEG_TO_IMAGE: {
		int x0 = currentCommand.params[0];
		int y0 = currentCommand.params[1];
		int width = currentCommand.params[2];
		int height = currentCommand.params[3];
		if (cntxt->depthBuffer.enabled == NMGL_TRUE) {
			cntxt->depthBuffer.setCursor(x0, y0);
			cntxt->smallDepthBuff.setCursor(0, 0);
			PROFILER_SIZE(width * height);
			cntxt->depthBuffer.popWindow(cntxt->smallDepthBuff, width, height);
		}
		cntxt->colorBuffer.setCursor(x0, y0);
		cntxt->smallColorBuff.setCursor(0, 0);
		PROFILER_SIZE(width * height);
		cntxt->colorBuffer.popWindow(cntxt->smallColorBuff, width, height);
		break;
	}

	case NMC1_DRAW_TRIANGLES: {
		Polygons* poly = cntxt->polygonsConnectors[0].ptrTail();
		PROFILER_SIZE(poly->count)
		drawTriangles();
		break;
	}

	case NMC1_DRAW_LINES: {
		drawLines();
		break;
	}
	case NMC1_DRAW_POINTS: {
		drawPoints();
		break;
	}
	case NMC1_POINT_SIZE: {
		cntxt->pointSize = currentCommand.params[0];
		break;
	}

	case NMC1_SET_COLOR: {
		int red = currentCommand.params[0];
		int green = currentCommand.params[1];
		int blue = currentCommand.params[2];
		int alpha = currentCommand.params[3];
		cntxt->colorBuffer.clearColor(red, green, blue, alpha);
		cntxt->smallColorBuff.mClearValue = cntxt->colorBuffer.mClearValue;
		cntxt->smallClearColorBuff.mClearValue = cntxt->colorBuffer.mClearValue;
		break;
	}
	case NMC1_SET_DEPTH:
		cntxt->depthBuffer.mClearValue = currentCommand.params[0];
		cntxt->smallDepthBuff.mClearValue = cntxt->depthBuffer.mClearValue;
		cntxt->smallClearDepthBuff.mClearValue = cntxt->depthBuffer.mClearValue;
		break;

	case NMC1_DEPTH_MASK: {
		bool maskEnabled = currentCommand.params[0];
		cntxt->depthBuffer.setEnabledMask(maskEnabled);
		break;
	}
	case NMC1_DEPTH_FUNC: {
		int depthMode = currentCommand.params[0];
		cntxt->depthBuffer.setMode(depthMode);
		break;
	}
	case NMC1_SWAP_BUFFER: {
		msdWaitDma();
		swapImage();
		break;
	}
	case NMC1_EXIT:
		exitNM1 = 1;
		break;

	case NMC1_DEPTH:
		cntxt->depthBuffer.enabled = currentCommand.params[0];
		break;
	
	case NMC1_SET_ACTIVE_TEXTURE: {
		cntxt->texState.activeTexUnitIndex = currentCommand.params[0];
		break;
	}
	
	case NMC1_SET_MIPMAP_LVL_POINTER: {
		//use DDR only as addresses are sent directly
		cntxt->texState.texObjects[currentCommand.params[0]].texImages2D[currentCommand.params[1]].pixels=(void*)currentCommand.params[2];
		DEBUG_PRINT2(("Step:for texture %d level %d got pointer %x\n",currentCommand.params[0],currentCommand.params[1],currentCommand.params[2]));
		break;
	}	
	case NMC1_BIND_ACTIVE_TEX_OBJECT: {
		NMGLenum target=currentCommand.params[0];
		NMGLuint texture=currentCommand.params[1];
        ActiveTexObjectP=&cntxt->texState.texObjects[texture];
		break;
	}
	case NMC1_SET_WHF: {
		#define TexObj cntxt->texState.texObjects
		NMGLuint texture=currentCommand.params[0];		
		NMGLsizei width0=currentCommand.params[1];
		NMGLsizei height0=currentCommand.params[2];
		NMGLenum format=currentCommand.params[3];
		int i=0;
		
		for(i=0;i<=NMGL_MAX_MIPMAP_LVL;i++)
		{
			TexObj[texture].texImages2D[i].internalformat=format;
			if(width0>>i >= 1)
			{
				TexObj[texture].texImages2D[i].width=width0>>i;
			}
			else
			{
				TexObj[texture].texImages2D[i].width=1;
			}
			if(height0>>i >= 1)
			{
				TexObj[texture].texImages2D[i].height=height0>>i;
			}
			else
			{
				TexObj[texture].texImages2D[i].height=1;
			}
		}
		break;
	}
	case NMC1_SET_COLOR_PALETTE: {
		cntxt->texState.texObjects[(int)currentCommand.params[2]].palette.colors=(NMGLubyte*)currentCommand.params[0];
		cntxt->texState.texObjects[(int)currentCommand.params[2]].palette.width=currentCommand.params[1];				
		break;
	}

	case NMC1_SET_TEX_ENV_COLOR: {
		Intfloat temp;
		for (int i = 0; i < 4; i++) {
			temp.i = currentCommand.params[i];
			cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[i] = temp.f;
		}
		break;
	}
	
	case NMC1_SET_TEX_ENV_MODE: {
		cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName = currentCommand.params[0];
		break;
	}

	case NMC1_SET_TEX_PARAMI: {
		switch ((NMGLenum)currentCommand.params[0])
		{
			case NMGL_TEXTURE_WRAP_S: 
				cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texWrapS = (NMGLint)currentCommand.params[1];
				break;
				
			case NMGL_TEXTURE_WRAP_T: 
				cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texWrapT = (NMGLint)currentCommand.params[1];
				break;
			
			case NMGL_TEXTURE_MIN_FILTER: 
				cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texMinFilter = (NMGLint)currentCommand.params[1];
				break;
			
			case NMGL_TEXTURE_MAG_FILTER: 
				cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texMagFilter = (NMGLint)currentCommand.params[1];
				break;
				
			default: 
				break;
		}
		break;
	}

	default:
		break;
	}
	return 0;
} 

