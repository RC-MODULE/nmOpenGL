#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

#define CHECK_SIZE()	if (cntxt.beginEndInfo.vertexCounter == cntxt.beginEndInfo.maxSize - 1) { \
		cntxt.error = NMGL_OUT_OF_MEMORY;	\
		return;								\
	}

SECTION(".text_nmgl")
void nmglVertex2f(NMGLfloat x, NMGLfloat y)
{
	CHECK_SIZE();
	int i = cntxt.beginEndInfo.vertexCounter;
	cntxt.beginEndInfo.vertex[i].vec[0] = x;
	cntxt.beginEndInfo.vertex[i].vec[1] = y;
	cntxt.beginEndInfo.vertex[i].vec[2] = 0;
	cntxt.beginEndInfo.vertex[i].vec[3] = 1;
	cntxt.beginEndInfo.vertexCounter++;
}

SECTION(".text_nmgl")
void nmglVertex3f(NMGLfloat x, NMGLfloat y, NMGLfloat z)
{
	CHECK_SIZE();
	int i = cntxt.beginEndInfo.vertexCounter;
	cntxt.beginEndInfo.vertex[i].vec[0] = x;
	cntxt.beginEndInfo.vertex[i].vec[1] = y;
	cntxt.beginEndInfo.vertex[i].vec[2] = z;
	cntxt.beginEndInfo.vertex[i].vec[3] = 1;
	cntxt.beginEndInfo.vertexCounter++;
}

SECTION(".text_nmgl")
void nmglVertex2fv(const NMGLfloat *v)
{
	CHECK_SIZE();
	int i = cntxt.beginEndInfo.vertexCounter;
	cntxt.beginEndInfo.vertex[i].vec[0] = v[0];
	cntxt.beginEndInfo.vertex[i].vec[1] = v[1];
	cntxt.beginEndInfo.vertex[i].vec[2] = 0;
	cntxt.beginEndInfo.vertex[i].vec[3] = 1;
	cntxt.beginEndInfo.vertexCounter++;
}

SECTION(".text_nmgl")
void nmglVertex3fv(const NMGLfloat *v)
{
	CHECK_SIZE();
	int i = cntxt.beginEndInfo.vertexCounter;
	printf("i=%d\n", i);
	cntxt.beginEndInfo.vertex[i].vec[0] = v[0];
	cntxt.beginEndInfo.vertex[i].vec[1] = v[1];
	cntxt.beginEndInfo.vertex[i].vec[2] = v[2];
	cntxt.beginEndInfo.vertex[i].vec[3] = 1;
	cntxt.beginEndInfo.vertexCounter++;
}
