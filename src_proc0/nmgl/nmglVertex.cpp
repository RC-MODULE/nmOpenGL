#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"

#pragma code_section ".text_nmgl"

#define CHECK_SIZE()	if (cntxt->beginEndInfo.vertexCounter == cntxt->beginEndInfo.maxSize - 1) { \
		cntxt->error = NMGL_OUT_OF_MEMORY;	\
		return;								\
	}

SECTION(".text_nmgl")
void nmglVertex2f(NMGLfloat x, NMGLfloat y)
{
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CHECK_SIZE();
	int i = cntxt->beginEndInfo.vertexCounter;
	cntxt->beginEndInfo.vertex[i].vec[0] = x;
	cntxt->beginEndInfo.vertex[i].vec[1] = y;
	cntxt->beginEndInfo.vertex[i].vec[2] = 0;
	cntxt->beginEndInfo.vertex[i].vec[3] = 1;

	cntxt->beginEndInfo.color[i].vec[0] = cntxt->currentColor.vec[0];
	cntxt->beginEndInfo.color[i].vec[1] = cntxt->currentColor.vec[1];
	cntxt->beginEndInfo.color[i].vec[2] = cntxt->currentColor.vec[2];
	cntxt->beginEndInfo.color[i].vec[3] = cntxt->currentColor.vec[3];

	cntxt->beginEndInfo.normal[i].vec[0] = cntxt->currentNormal.vec[0];
	cntxt->beginEndInfo.normal[i].vec[1] = cntxt->currentNormal.vec[1];
	cntxt->beginEndInfo.normal[i].vec[2] = cntxt->currentNormal.vec[2];
	cntxt->beginEndInfo.normal[i].vec[3] = cntxt->currentNormal.vec[3];

	//TEXTURING_PART
	//XXX: Only one texture unit is supported.
	cntxt->beginEndInfo.texcoord[i].v0 = cntxt->texState.curTexCoords[0].s;
	cntxt->beginEndInfo.texcoord[i].v1 = cntxt->texState.curTexCoords[0].t;
	//TEXTURING_PART
	
	cntxt->beginEndInfo.vertexCounter++;
}

SECTION(".text_nmgl")
void nmglVertex3f(NMGLfloat x, NMGLfloat y, NMGLfloat z)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CHECK_SIZE();
	int i = cntxt->beginEndInfo.vertexCounter;
	cntxt->beginEndInfo.vertex[i].vec[0] = x;
	cntxt->beginEndInfo.vertex[i].vec[1] = y;
	cntxt->beginEndInfo.vertex[i].vec[2] = z;
	cntxt->beginEndInfo.vertex[i].vec[3] = 1;

	cntxt->beginEndInfo.color[i].vec[0] = cntxt->currentColor.vec[0];
	cntxt->beginEndInfo.color[i].vec[1] = cntxt->currentColor.vec[1];
	cntxt->beginEndInfo.color[i].vec[2] = cntxt->currentColor.vec[2];
	cntxt->beginEndInfo.color[i].vec[3] = cntxt->currentColor.vec[3];

	cntxt->beginEndInfo.normal[i].vec[0] = cntxt->currentNormal.vec[0];
	cntxt->beginEndInfo.normal[i].vec[1] = cntxt->currentNormal.vec[1];
	cntxt->beginEndInfo.normal[i].vec[2] = cntxt->currentNormal.vec[2];
	cntxt->beginEndInfo.normal[i].vec[3] = cntxt->currentNormal.vec[3];
	
	//TEXTURING_PART
	//XXX: Only one texture unit is supported.
	cntxt->beginEndInfo.texcoord[i].v0 = cntxt->texState.curTexCoords[0].s;
	cntxt->beginEndInfo.texcoord[i].v1 = cntxt->texState.curTexCoords[0].t;
	//TEXTURING_PART

	cntxt->beginEndInfo.vertexCounter++;
}

SECTION(".text_nmgl")
void nmglVertex2fv(const NMGLfloat *v)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CHECK_SIZE();
	int i = cntxt->beginEndInfo.vertexCounter;
	cntxt->beginEndInfo.vertex[i].vec[0] = v[0];
	cntxt->beginEndInfo.vertex[i].vec[1] = v[1];
	cntxt->beginEndInfo.vertex[i].vec[2] = 0;
	cntxt->beginEndInfo.vertex[i].vec[3] = 1;

	cntxt->beginEndInfo.color[i].vec[0] = cntxt->currentColor.vec[0];
	cntxt->beginEndInfo.color[i].vec[1] = cntxt->currentColor.vec[1];
	cntxt->beginEndInfo.color[i].vec[2] = cntxt->currentColor.vec[2];
	cntxt->beginEndInfo.color[i].vec[3] = cntxt->currentColor.vec[3];

	cntxt->beginEndInfo.normal[i].vec[0] = cntxt->currentNormal.vec[0];
	cntxt->beginEndInfo.normal[i].vec[1] = cntxt->currentNormal.vec[1];
	cntxt->beginEndInfo.normal[i].vec[2] = cntxt->currentNormal.vec[2];
	cntxt->beginEndInfo.normal[i].vec[3] = cntxt->currentNormal.vec[3];
	
	//TEXTURING_PART
	//XXX: Only one texture unit is supported.
	cntxt->beginEndInfo.texcoord[i].v0 = cntxt->texState.curTexCoords[0].s;
	cntxt->beginEndInfo.texcoord[i].v1 = cntxt->texState.curTexCoords[0].t;
	//TEXTURING_PART

	cntxt->beginEndInfo.vertexCounter++;
}

SECTION(".text_nmgl")
void nmglVertex3fv(const NMGLfloat *v)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CHECK_SIZE();
	int i = cntxt->beginEndInfo.vertexCounter;
	cntxt->beginEndInfo.vertex[i].vec[0] = v[0];
	cntxt->beginEndInfo.vertex[i].vec[1] = v[1];
	cntxt->beginEndInfo.vertex[i].vec[2] = v[2];
	cntxt->beginEndInfo.vertex[i].vec[3] = 1;

	cntxt->beginEndInfo.color[i].vec[0] = cntxt->currentColor.vec[0];
	cntxt->beginEndInfo.color[i].vec[1] = cntxt->currentColor.vec[1];
	cntxt->beginEndInfo.color[i].vec[2] = cntxt->currentColor.vec[2];
	cntxt->beginEndInfo.color[i].vec[3] = cntxt->currentColor.vec[3];

	cntxt->beginEndInfo.normal[i].vec[0] = cntxt->currentNormal.vec[0];
	cntxt->beginEndInfo.normal[i].vec[1] = cntxt->currentNormal.vec[1];
	cntxt->beginEndInfo.normal[i].vec[2] = cntxt->currentNormal.vec[2];
	cntxt->beginEndInfo.normal[i].vec[3] = cntxt->currentNormal.vec[3];
	
	//TEXTURING_PART
	//XXX: Only one texture unit is supported.
	cntxt->beginEndInfo.texcoord[i].v0 = cntxt->texState.curTexCoords[0].s;
	cntxt->beginEndInfo.texcoord[i].v1 = cntxt->texState.curTexCoords[0].t;
	//TEXTURING_PART

	cntxt->beginEndInfo.vertexCounter++;
}
