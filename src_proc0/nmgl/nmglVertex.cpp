#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

#define CHECK_SIZE()	if (context->beginEndInfo.vertexCounter == context->beginEndInfo.maxSize - 1) { \
		cntxt->error = NMGL_OUT_OF_MEMORY;	\
		return;								\
	}

SECTION(".text_nmgl")
void nmglVertex2f(NMGLfloat x, NMGLfloat y)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	CHECK_SIZE();
	int i = context->beginEndInfo.vertexCounter;
	context->beginEndInfo.vertex[i].vec[0] = x;
	context->beginEndInfo.vertex[i].vec[1] = y;
	context->beginEndInfo.vertex[i].vec[2] = 0;
	context->beginEndInfo.vertex[i].vec[3] = 1;

	context->beginEndInfo.color[i].vec[0] = context->currentColor.vec[0];
	context->beginEndInfo.color[i].vec[1] = context->currentColor.vec[1];
	context->beginEndInfo.color[i].vec[2] = context->currentColor.vec[2];
	context->beginEndInfo.color[i].vec[3] = context->currentColor.vec[3];

	context->beginEndInfo.normal[i].vec[0] = context->currentNormal.vec[0];
	context->beginEndInfo.normal[i].vec[1] = context->currentNormal.vec[1];
	context->beginEndInfo.normal[i].vec[2] = context->currentNormal.vec[2];
	context->beginEndInfo.normal[i].vec[3] = context->currentNormal.vec[3];

	//TEXTURING_PART
	//XXX: Only one texture unit is supported.
	context->beginEndInfo.texcoord[i].v0 = cntxt->texState.curTexCoords[0].s;
	context->beginEndInfo.texcoord[i].v1 = cntxt->texState.curTexCoords[0].t;
	//TEXTURING_PART

	context->beginEndInfo.vertexCounter++;
}

SECTION(".text_nmgl")
void nmglVertex3f(NMGLfloat x, NMGLfloat y, NMGLfloat z)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();
	CHECK_SIZE();
	int i = context->beginEndInfo.vertexCounter;
	context->beginEndInfo.vertex[i].vec[0] = x;
	context->beginEndInfo.vertex[i].vec[1] = y;
	context->beginEndInfo.vertex[i].vec[2] = z;
	context->beginEndInfo.vertex[i].vec[3] = 1;

	context->beginEndInfo.color[i].vec[0] = context->currentColor.vec[0];
	context->beginEndInfo.color[i].vec[1] = context->currentColor.vec[1];
	context->beginEndInfo.color[i].vec[2] = context->currentColor.vec[2];
	context->beginEndInfo.color[i].vec[3] = context->currentColor.vec[3];

	context->beginEndInfo.normal[i].vec[0] = context->currentNormal.vec[0];
	context->beginEndInfo.normal[i].vec[1] = context->currentNormal.vec[1];
	context->beginEndInfo.normal[i].vec[2] = context->currentNormal.vec[2];
	context->beginEndInfo.normal[i].vec[3] = context->currentNormal.vec[3];
	
	//TEXTURING_PART
	//XXX: Only one texture unit is supported.
	context->beginEndInfo.texcoord[i].v0 = cntxt->texState.curTexCoords[0].s;
	context->beginEndInfo.texcoord[i].v1 = cntxt->texState.curTexCoords[0].t;
	//TEXTURING_PART

	context->beginEndInfo.vertexCounter++;
}

SECTION(".text_nmgl")
void nmglVertex2fv(const NMGLfloat *v)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();
	CHECK_SIZE();
	int i = context->beginEndInfo.vertexCounter;
	context->beginEndInfo.vertex[i].vec[0] = v[0];
	context->beginEndInfo.vertex[i].vec[1] = v[1];
	context->beginEndInfo.vertex[i].vec[2] = 0;
	context->beginEndInfo.vertex[i].vec[3] = 1;

	context->beginEndInfo.color[i].vec[0] = context->currentColor.vec[0];
	context->beginEndInfo.color[i].vec[1] = context->currentColor.vec[1];
	context->beginEndInfo.color[i].vec[2] = context->currentColor.vec[2];
	context->beginEndInfo.color[i].vec[3] = context->currentColor.vec[3];

	context->beginEndInfo.normal[i].vec[0] = context->currentNormal.vec[0];
	context->beginEndInfo.normal[i].vec[1] = context->currentNormal.vec[1];
	context->beginEndInfo.normal[i].vec[2] = context->currentNormal.vec[2];
	context->beginEndInfo.normal[i].vec[3] = context->currentNormal.vec[3];
	
	//TEXTURING_PART
	//XXX: Only one texture unit is supported.
	context->beginEndInfo.texcoord[i].v0 = cntxt->texState.curTexCoords[0].s;
	context->beginEndInfo.texcoord[i].v1 = cntxt->texState.curTexCoords[0].t;
	//TEXTURING_PART

	context->beginEndInfo.vertexCounter++;
}

SECTION(".text_nmgl")
void nmglVertex3fv(const NMGLfloat *v)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();
	CHECK_SIZE();
	int i = context->beginEndInfo.vertexCounter;
	context->beginEndInfo.vertex[i].vec[0] = v[0];
	context->beginEndInfo.vertex[i].vec[1] = v[1];
	context->beginEndInfo.vertex[i].vec[2] = v[2];
	context->beginEndInfo.vertex[i].vec[3] = 1;

	context->beginEndInfo.color[i].vec[0] = context->currentColor.vec[0];
	context->beginEndInfo.color[i].vec[1] = context->currentColor.vec[1];
	context->beginEndInfo.color[i].vec[2] = context->currentColor.vec[2];
	context->beginEndInfo.color[i].vec[3] = context->currentColor.vec[3];

	context->beginEndInfo.normal[i].vec[0] = context->currentNormal.vec[0];
	context->beginEndInfo.normal[i].vec[1] = context->currentNormal.vec[1];
	context->beginEndInfo.normal[i].vec[2] = context->currentNormal.vec[2];
	context->beginEndInfo.normal[i].vec[3] = context->currentNormal.vec[3];
	
	//TEXTURING_PART
	//XXX: Only one texture unit is supported.
	context->beginEndInfo.texcoord[i].v0 = cntxt->texState.curTexCoords[0].s;
	context->beginEndInfo.texcoord[i].v1 = cntxt->texState.curTexCoords[0].t;
	//TEXTURING_PART

	context->beginEndInfo.vertexCounter++;
}
