/*!
 *  \file rasterizeT.cpp
 *  \author Zhilenkov Ivan
 */
#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"

#include "stdio.h"


SECTION(".text_demo3d") void triangleOffset(Triangles &src, Triangles &dst, int offset) {
	dst.x0 = src.x0 + offset;
	dst.y0 = src.y0 + offset;
	dst.x1 = src.x1 + offset;
	dst.y1 = src.y1 + offset;
	dst.x2 = src.x2 + offset;
	dst.y2 = src.y2 + offset;
	dst.z = src.z + offset;
	dst.colors = src.colors + offset;
#ifdef TEXTURE_ENABLED
	//TODO: May be remove check cntxt->texState.textureEnabled value
	//if it is slowing down pipeline more than extra sum.
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (cntxt->texState.textureEnabled) {
		dst.s0 = src.s0 + offset;
		dst.t0 = src.t0 + offset;
		dst.s1 = src.s1 + offset;
		dst.t1 = src.t1 + offset;
		dst.s2 = src.s2 + offset;
		dst.t2 = src.t2 + offset;
		dst.w0 = src.w0 + offset;
		dst.w1 = src.w1 + offset;
		dst.w2 = src.w2 + offset;
	}
#endif //TEXTURE_ENABLED
}

SECTION(".text_demo3d")
void rasterizeT(const Triangles* triangles, const BitMask* masks){

	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

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

#ifdef TEXTURE_ENABLED
	if (cntxt->texState.textureEnabled) {
		localTrian.s0 = cntxt->buffer0 + 10 * NMGL_SIZE;
		localTrian.t0 = cntxt->buffer0 + 11 * NMGL_SIZE;
		localTrian.s1 = cntxt->buffer1 + 10 * NMGL_SIZE;
		localTrian.t1 = cntxt->buffer1 + 11 * NMGL_SIZE;
		localTrian.s2 = cntxt->buffer2 + 10 * NMGL_SIZE;
		localTrian.t2 = cntxt->buffer2 + 11 * NMGL_SIZE;
		localTrian.w0 = cntxt->buffer3 +  9 * NMGL_SIZE;
		localTrian.w1 = cntxt->buffer3 + 10 * NMGL_SIZE;
		localTrian.w2 = cntxt->buffer3 + 11 * NMGL_SIZE;
	}
#endif //TEXTURE_ENABLED


	for (int segY = 0, iSeg = 0; segY < cntxt->windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt->windowInfo.nColumns; segX++, iSeg++) {
			if (masks[iSeg].hasNotZeroBits != 0) {
				int resultSize = readMask(masks[iSeg].bits, indices, count);
				if (resultSize) {
					PolygonsConnector *connector = cntxt->triangleConnectors + iSeg;
					bool drawingCheck = connector->ptrHead()->count + resultSize >= POLYGONS_SIZE;
					if (drawingCheck) {
						cntxt->synchro.writeInstr(1, NMC1_COPY_SEG_FROM_IMAGE,
							cntxt->windowInfo.x0[segX],
							cntxt->windowInfo.y0[segY],
							cntxt->windowInfo.x1[segX] - cntxt->windowInfo.x0[segX],
							cntxt->windowInfo.y1[segY] - cntxt->windowInfo.y0[segY],
							iSeg);
					}

#ifdef TEXTURE_ENABLED
					if (cntxt->texState.textureEnabled) {
						copyArraysByIndices((void**)triangles, indices, (void**)&localTrian, 16, resultSize);
					}
					else {
						copyArraysByIndices((void**)triangles, indices, (void**)&localTrian, 7, resultSize);
					}
#else //TEXTURE_ENABLED
					copyArraysByIndices((void**)triangles, indices, (void**)&localTrian, 7, resultSize);
#endif //TEXTURE_ENABLED
					copyColorByIndices_BGRA_RGBA(triangles->colors, indices, (v4nm32s*)localTrian.colors, resultSize);
					localTrian.size = resultSize;

					int offset = 0;
					Triangles localTrian2;
					while (offset < resultSize) {
						DataForNmpu1* data = connector->ptrHead();
						int localSize = MIN(resultSize - offset, POLYGONS_SIZE - data->count);
						triangleOffset(localTrian, localTrian2, offset);
						offset += localSize;
						updatePolygonsT(data, &localTrian2, localSize, segX, segY);
						if (data->count == POLYGONS_SIZE) {
							transferPolygons(connector, NMC1_DRAW_TRIANGLES);
						}
					}
					if (drawingCheck) {
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
	}
}
