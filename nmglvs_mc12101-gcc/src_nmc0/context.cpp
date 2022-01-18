#include "context.h"
#include "demo3d_nm0.h"
#include "section-hal.h"
#include "ringbuffert.h"
#include "framebuffer.h"

INSECTION(".data_imu0") NMGL_Context *globalContext = 0;
INSECTION(".data_imu0") int contextIsModified = true;
INSECTION(".data_imu0") int contextLock = false;

void NMGL_FrameBufferInit(NMGL_FrameBuffer *fb, int width, int height){
	fb->head = 0;
	fb->tail = 0;
	fb->sizeOfAddr = sizeof(void *);
	fb->sizeOfInt = sizeof(int);
	fb->width = width;
	fb->height = height;
}

NMGL_Context *NMGL_CreateContext(NMGL_ContextConfig *config)
{
	setHeap(10);
	NMGL_Context *context = (NMGL_Context *)halMalloc32(sizeof32(NMGL_Context));
	globalContext = context;

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

	return context;
}

NMGL_Context *NMGL_GetCurrentContext(){
	while (contextLock);
	return globalContext;
}


