#include "demo3d_nm0.h"
#include "nmgl.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglFlush (){

//#ifdef USED_POLYGONS_BUFFER
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	for (int segY = 0, iSeg = 0; segY < cntxt->windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt->windowInfo.nColumns; segX++, iSeg++) {
			PolygonsConnector *trian_connector = cntxt->triangleConnectors + iSeg;
			PolygonsConnector *line_connector = cntxt->lineConnectors + iSeg;
			PolygonsConnector *point_connector = cntxt->pointConnectors + iSeg;
			bool drawingCheck = trian_connector->ptrHead()->count > 0 ||
								line_connector->ptrHead()->count > 0 ||
								point_connector->ptrHead()->count > 0;
			if (drawingCheck) {
				cntxt->synchro.writeInstr(1, NMC1_COPY_SEG_FROM_IMAGE,
					cntxt->windowInfo.x0[segX],
					cntxt->windowInfo.y0[segY],
					cntxt->windowInfo.x1[segX] - cntxt->windowInfo.x0[segX],
					cntxt->windowInfo.y1[segY] - cntxt->windowInfo.y0[segY],
					iSeg);
			}

			if (trian_connector->ptrHead()->count) {
				transferPolygons(trian_connector, NMC1_DRAW_TRIANGLES);
			}
			if (line_connector->ptrHead()->count) {
				transferPolygons(line_connector, NMC1_DRAW_LINES);
			}
			if (point_connector->ptrHead()->count) {
				transferPolygons(point_connector, NMC1_DRAW_POINTS);
			}

			if(drawingCheck){
				cntxt->synchro.writeInstr(1,
					NMC1_COPY_SEG_TO_IMAGE,
					cntxt->windowInfo.x0[segX],
					cntxt->windowInfo.y0[segY],
					cntxt->windowInfo.x1[segX] - cntxt->windowInfo.x0[segX],
					cntxt->windowInfo.y1[segY] - cntxt->windowInfo.y0[segY]);
			}
		}
	}
//#endif // USED_POLYGONS_BUFFER

}
