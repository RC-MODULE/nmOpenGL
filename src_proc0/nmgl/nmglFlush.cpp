#include "demo3d_nm0.h"
#include "nmgl.h"

#include <math.h>

extern  Polygons polygons[36];
void pushPoly(Polygons &innerPoly, PolygonsConnector &connector);

SECTION(".text_nmgl")
void nmglFlush (){

#ifdef USED_POLYGONS_BUFFER
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	PolygonsConnector connector(cntxt->polygonsData);

	for (int segY = 0, iSeg = 0; segY < cntxt->windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt->windowInfo.nColumns; segX++, iSeg++) {
			if (polygons[iSeg].count) {
				cntxt->synchro.writeInstr(1, NMC1_COPY_SEG_FROM_IMAGE,
					cntxt->windowInfo.x0[segX],
					cntxt->windowInfo.y0[segY],
					cntxt->windowInfo.x1[segX] - cntxt->windowInfo.x0[segX],
					cntxt->windowInfo.y1[segY] - cntxt->windowInfo.y0[segY],
					iSeg);

				pushPoly(polygons[iSeg], connector);

				cntxt->synchro.writeInstr(1,
					NMC1_COPY_SEG_TO_IMAGE,
					cntxt->windowInfo.x0[segX],
					cntxt->windowInfo.y0[segY],
					cntxt->windowInfo.x1[segX] - cntxt->windowInfo.x0[segX],
					cntxt->windowInfo.y1[segY] - cntxt->windowInfo.y0[segY]);
			}
		}
	}
#endif // USED_POLYGONS_BUFFER

}
