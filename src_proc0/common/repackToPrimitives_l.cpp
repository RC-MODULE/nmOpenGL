#include "nmpp.h"
#include "demo3d_nm0.h"

#include "nmblas.h"

SECTION(".text_demo3d")
int repackToPrimitives_l(const v4nm32f *srcVertex,	const v4nm32f *srcColor, const v2nm32f *srcTex,	LinePointers *dstPointers, int vertexAmount){
	int result = vertexAmount / 2;
    for (int i = 0; i < result; i++)
    {
        dstPointers->v0.x[i]  = srcVertex[2 * i + 0].vec[0];
        dstPointers->v0.y[i]  = srcVertex[2 * i + 0].vec[1];
        dstPointers->v0.z[i]  = srcVertex[2 * i + 0].vec[2];
        dstPointers->v0.w[i]  = srcVertex[2 * i + 0].vec[3];
		dstPointers->v0.color[i] = srcColor[2 * i + 0];

        dstPointers->v1.x[i]  = srcVertex[2 * i + 1].vec[0];
        dstPointers->v1.y[i]  = srcVertex[2 * i + 1].vec[1];
        dstPointers->v1.z[i]  = srcVertex[2 * i + 1].vec[2];
        dstPointers->v1.w[i]  = srcVertex[2 * i + 1].vec[3];
		dstPointers->v1.color[i]  = srcColor[2 * i + 1];

		// TEXTURING_PART
		dstPointers->v0.s[i] = srcTex[2 * i + 0].v0;
		dstPointers->v0.t[i] = srcTex[2 * i + 0].v1;
		dstPointers->v1.s[i] = srcTex[2 * i + 1].v0;
		dstPointers->v1.t[i] = srcTex[2 * i + 1].v1;
		// TEXTURING_PART
	}
	
	if(result % 2){
		dstPointers->v0.x[result] = dstPointers->v0.x[result - 1];
		dstPointers->v0.y[result] = dstPointers->v0.y[result - 1];
		dstPointers->v0.z[result] = dstPointers->v0.z[result - 1];
		dstPointers->v0.w[result] = dstPointers->v0.w[result - 1];
		dstPointers->v0.color[result] = dstPointers->v0.color[result - 1];
		
		dstPointers->v1.x[result] = dstPointers->v1.x[result - 1];
		dstPointers->v1.y[result] = dstPointers->v1.y[result - 1];
		dstPointers->v1.z[result] = dstPointers->v1.z[result - 1];
		dstPointers->v1.w[result] = dstPointers->v1.w[result - 1];
		dstPointers->v1.color[result] = dstPointers->v1.color[result - 1];

		// TEXTURING_PART
		dstPointers->v0.s[result] = dstPointers->v0.s[result - 1];
		dstPointers->v0.t[result] = dstPointers->v0.t[result - 1];
		dstPointers->v1.s[result] = dstPointers->v1.s[result - 1];
		dstPointers->v1.t[result] = dstPointers->v1.t[result - 1];
		// TEXTURING_PART
		
		result++;
	}	
	
	return result;
}
