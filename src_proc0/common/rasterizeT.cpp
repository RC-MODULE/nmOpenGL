#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"

#include "stdio.h"
#include "nmprofiler.h"

extern  Polygons polygons[36];


SECTION(".text_demo3d") void triangleOffset(Triangles &src, Triangles &dst, int offset) {
	dst.x0 = src.x0 + offset;
	dst.y0 = src.y0 + offset;
	dst.x1 = src.x1 + offset;
	dst.y1 = src.y1 + offset;
	dst.x2 = src.x2 + offset;
	dst.y2 = src.y2 + offset;
	dst.z = src.z + offset;
	dst.colors = src.colors + offset;
}

/*SECTION(".text_demo3d") void pushPoly(Polygons &innerPoly, PolygonsConnector &connector) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	while (connector.isFull());
	nmblas_scopy(sizeof32(Polygons), (float*)&innerPoly, 1, (float*)connector.ptrHead(), 1);
	innerPoly.count = 0;
	connector.incHead();
	cntxt->synchro.writeInstr(1, NMC1_DRAW_TRIANGLES);
}

SECTION(".text_demo3d") void recursiveUpdate(Triangles& triangles, Polygons &polygons, PolygonsConnector &connector, int segX, int segY) {
	if (triangles.size + polygons.count < POLYGONS_SIZE) {
		updatePolygonsT(&polygons, &triangles, triangles.size, segX, segY);
	}
	else {
		Triangles tmpTrian;
		int count = POLYGONS_SIZE - polygons.count;
		updatePolygonsT(&polygons, &triangles, count, segX, segY);
		pushPoly(polygons, connector);
		triangleOffset(triangles, tmpTrian, count);
		tmpTrian.size = triangles.size - count;
		recursiveUpdate(tmpTrian, polygons, connector, segX, segY);
	}
}*/

SECTION(".text_demo3d")
void rasterizeT(const Triangles* triangles, const BitMask* masks){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	PolygonsConnector *connector = cntxt->polygonsConnectors;

	int count = triangles->size;

	Triangles localTrian;
	localTrian.x0 = cntxt->buffer0;
	localTrian.y0 = cntxt->buffer0 + NMGL_SIZE;
	localTrian.x1 = cntxt->buffer1;
	localTrian.y1 = cntxt->buffer1 + NMGL_SIZE;
	localTrian.x2 = cntxt->buffer2;
	localTrian.y2 = cntxt->buffer2 + NMGL_SIZE;
	localTrian.colors = (v4nm32s*)cntxt->buffer3;
	localTrian.z = (int*)cntxt->buffer3 + 4 * NMGL_SIZE;
	int* indices = (int*)cntxt->buffer4;

	for (int segY = 0, iSeg = 0; segY < cntxt->windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt->windowInfo.nColumns; segX++, iSeg++) {
			if (masks[iSeg].hasNotZeroBits != 0) {

				int resultSize = readMask(masks[iSeg].bits, indices, count);
				
				cntxt->synchro.writeInstr(1, NMC1_COPY_SEG_FROM_IMAGE,
					cntxt->windowInfo.x0[segX],
					cntxt->windowInfo.y0[segY],
					cntxt->windowInfo.x1[segX] - cntxt->windowInfo.x0[segX],
					cntxt->windowInfo.y1[segY] - cntxt->windowInfo.y0[segY],
					iSeg);

				copyArraysByIndices((void**)triangles, indices, (void**)&localTrian, 7, resultSize);

#ifdef OUTPUT_IMAGE_RGB8888
				copyColorByIndices_BGRA_RGBA(triangles->colors, indices, (v4nm32s*)localTrian.colors, resultSize);
#endif // OUTPUT_IMAGE_RGB8888
#ifdef OUTPUT_IMAGE_RGB565
				copyColorByIndices(triangles->colors, indices, (v4nm32s*)localTrian.colors, resultSize);
#endif // OUTPUT_IMAGE_RGB565

				localTrian.size = resultSize;
					
				int offset = 0;
				Triangles localTrian2;
				while (offset < resultSize) {
					int localSize = MIN(resultSize - offset, POLYGONS_SIZE);
					triangleOffset(localTrian, localTrian2, offset);
					offset += POLYGONS_SIZE;
					while (connector[0].isFull()) {
						halSleep(2);
					}
					Polygons* poly = connector[0].ptrHead();
					poly->count = 0;
					PROFILER_SIZE(localSize);
					updatePolygonsT(poly, &localTrian2, localSize, segX, segY);
					connector[0].incHead();
					cntxt->synchro.writeInstr(1, NMC1_DRAW_TRIANGLES);
				}	

				cntxt->synchro.writeInstr(1,
					NMC1_COPY_SEG_TO_IMAGE,
					cntxt->windowInfo.x0[segX],
					cntxt->windowInfo.y0[segY],
					cntxt->windowInfo.x1[segX] - cntxt->windowInfo.x0[segX],
					cntxt->windowInfo.y1[segY] - cntxt->windowInfo.y0[segY]);
					
			}
		}
	}

}