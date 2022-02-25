#include "nmpp.h"
#include "primitive.h"

#include "nmblas.h"

extern "C" {
int repackToPrimitives_tf(const v4nm32f *srcVertex,	const v4nm32f *srcColor, const v2nm32f *srcTex,	TrianglePointers *dstPointers, int vertexAmount){
	int result = vertexAmount - 2;
    for (int i = 0; i < result; i++)
    {
        dstPointers->v0.x[i]  = srcVertex[0].vec[0];
        dstPointers->v0.y[i]  = srcVertex[0].vec[1];
        dstPointers->v0.z[i]  = srcVertex[0].vec[2];
        dstPointers->v0.w[i]  = srcVertex[0].vec[3];
		dstPointers->v0.color[i] = srcColor[0];

        dstPointers->v1.x[i]  = srcVertex[(i + 1)].vec[0];
        dstPointers->v1.y[i]  = srcVertex[(i + 1)].vec[1];
        dstPointers->v1.z[i]  = srcVertex[(i + 1)].vec[2];
        dstPointers->v1.w[i]  = srcVertex[(i + 1)].vec[3];
		dstPointers->v1.color[i]  = srcColor[(i + 1)];

        dstPointers->v2.x[i]  = srcVertex[(i + 2)].vec[0];
        dstPointers->v2.y[i]  = srcVertex[(i + 2)].vec[1];
        dstPointers->v2.z[i]  = srcVertex[(i + 2)].vec[2];
        dstPointers->v2.w[i]  = srcVertex[(i + 2)].vec[3];
		dstPointers->v2.color[i] = srcColor[(i + 2)];

		//TEXTURING_PART
		dstPointers->v0.s[i] = srcTex[0].v0;
		dstPointers->v0.t[i] = srcTex[0].v1;
		dstPointers->v1.s[i] = srcTex[i + 1].v0;
		dstPointers->v1.t[i] = srcTex[i + 1].v1;		
		dstPointers->v2.s[i] = srcTex[i + 2].v0;
		dstPointers->v2.t[i] = srcTex[i + 2].v1;
		//TEXTURING_PART
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
		
		
		dstPointers->v2.x[result] = dstPointers->v2.x[result - 1];
		dstPointers->v2.y[result] = dstPointers->v2.y[result - 1];
		dstPointers->v2.z[result] = dstPointers->v2.z[result - 1];
		dstPointers->v2.w[result] = dstPointers->v2.w[result - 1];
		dstPointers->v2.color[result] = dstPointers->v2.color[result - 1];
		//TEXTURING_PART
		dstPointers->v0.s[result] = dstPointers->v0.s[result - 1];
		dstPointers->v0.t[result] = dstPointers->v0.t[result - 1];
		dstPointers->v1.s[result] = dstPointers->v1.s[result - 1];
		dstPointers->v1.t[result] = dstPointers->v1.t[result - 1];
		dstPointers->v2.s[result] = dstPointers->v2.s[result - 1];
		dstPointers->v2.t[result] = dstPointers->v2.t[result - 1];
		//TEXTURING_PART
		result++;
	}	
	
	return result;
}

}
