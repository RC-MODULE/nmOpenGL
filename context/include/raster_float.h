#include "nmgltype.h"
#include "context.h"
#include "primitive.h"


struct NMGL_CullFace{
	NMGLenum type;
	NMGLenum frontface;
};


int cullFaceSortTriangles(NMGL_CullFace &cullface, TrianglePointers &triangles, int count);