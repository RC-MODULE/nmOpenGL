#include "demo3d_nm0.h"
#include "nmblas.h"

SECTION(".text_demo3d") void copyTriangles(const Triangles &src, int offsetSrc, Triangles &dst, int offsetDst, int size) {
	nmblas_scopy(size, &src.x0[offsetSrc], 1, &dst.x0[offsetDst], 1);
	nmblas_scopy(size, &src.y0[offsetSrc], 1, &dst.y0[offsetDst], 1);
	nmblas_scopy(size, &src.x1[offsetSrc], 1, &dst.x1[offsetDst], 1);
	nmblas_scopy(size, &src.y1[offsetSrc], 1, &dst.y1[offsetDst], 1);
	nmblas_scopy(size, &src.x2[offsetSrc], 1, &dst.x2[offsetDst], 1);
	nmblas_scopy(size, &src.y2[offsetSrc], 1, &dst.y2[offsetDst], 1);
	nmblas_scopy(size, (float*)&src.z[offsetSrc], 1, (float*)&dst.z[offsetDst], 1);
	nmblas_scopy(4 * size, (float*)&src.colors[offsetSrc], 1, (float*)&dst.colors[offsetDst], 1);
}