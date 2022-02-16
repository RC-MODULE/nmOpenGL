#include "nmgltype.h"
#include "context.h"

struct NMGL_CullFace{
	NMGLenum type;
	NMGLenum frontface;
};


int cullFaceSortTriangles(NMGL_CullFace *cullface, TrianglePointers &triangles, int count);