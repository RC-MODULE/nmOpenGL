#include "nmpp.h"
#include "demo3d_nm0.h"

#include "nmblas.h"


SECTION(".text_demo3d")
int repackToPrimitives_t(const v4nm32f *srcVertex,	const v4nm32f *srcColor, const v2nm32f *srcTex,	TrianglePointers *dstPointers, int vertexAmount){
#ifdef TEXTURE_ENABLED
	return repackToPrimitives_t_full(srcVertex, srcColor, srcTex, dstPointers, vertexAmount);
#else
	return repackToPrimitives_t_without_textures(srcVertex, srcColor, srcTex, dstPointers, vertexAmount);
#endif
}
