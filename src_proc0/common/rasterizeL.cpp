#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "stdio.h"
#include "nmprofiler.h"



SECTION(".text_demo3d")
void rasterizeL(Lines* lines, int count){

	/*float* minX = cntxt.buffer0;
	float* maxX = cntxt.buffer1;
	float* minY = cntxt.buffer2;
	float* maxY = cntxt.buffer3;
	findMinMax2(lines->x0, lines->x1, minX, maxX, count);
	findMinMax2(lines->y0, lines->y1, minY, maxY, count);
	nmppsMerge_32f(minX, minY, minXY, count);
	nmppsMerge_32f(maxX, maxY, maxXY, count);

	for (int segY = 0; segY < cntxt.windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt.windowInfo.nColumns; segX++) {
			int polygonsCheck = 0;
			v2nm32f* lowerLimit = (v2nm32f*)&cntxt.tmp;
			v2nm32f* upperLimit = (v2nm32f*)&cntxt.tmp.vec[2];
			lowerLimit->v0 = cntxt.windowInfo.x0_f[segX];
			lowerLimit->v1 = cntxt.windowInfo.y0_f[segY];
			upperLimit->v0 = cntxt.windowInfo.x1_f[segX];
			upperLimit->v1 = cntxt.windowInfo.y1_f[segY];

			nmppsCmpLtC_v2nm32f((v2nm32f*)minXY, (v2nm32f*)upperLimit, masks.xLt, masks.yLt, 1, count);
			nmppsCmpGtC_v2nm32f((v2nm32f*)maxXY, (v2nm32f*)lowerLimit, masks.xGt, masks.yGt, 1, count);
			polygonsCheck = andMask(&masks, maskRes, count);
			if (polygonsCheck) {
				addInstrNMC1(&cntxt.synchro.commandsRB,
					NMC1_COPY_SEG_FROM_IMAGE,
					cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y0[segY],
					cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY]);

				while (halRingBufferIsFull(nmglPolygonsRB));
				Polygons* poly = (Polygons*)halRingBufferHead(nmglPolygonsRB);

				localLines.x1 = cntxt.buffer1;
				localLines.y1 = cntxt.buffer1 + NMGL_SIZE;
				localLines.x0 = cntxt.buffer2;
				localLines.y0 = cntxt.buffer2 + NMGL_SIZE;
				localLines.z = poly->z;
				localLines.colors = (v4nm32s*)poly->color;
				
				int localSize = copyArraysByMask((void**)lines, (nm1*)maskRes, (void**)&localLines, 5, count);
				maskSelectionLight_RGBA_BGRA((v4nm32s*)lines->colors, (nm1*)maskRes, (v4nm32s*)poly->color, count);
				fillPolygonsL(poly, &localLines, localSize, segX, segY);

				nmglPolygonsRB->head++;
				addInstrNMC1(&cntxt.synchro.commandsRB, NMC1_DRAW_TRIANGLES);

				addInstrNMC1(&cntxt.synchro.commandsRB,
					NMC1_COPY_SEG_TO_IMAGE,
					cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y0[segY],
					cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY]);
				
			}
		}
	}*/
}