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

			CommandNm1 command;
			if (drawingCheck) {
				command.instr = NMC1_COPY_SEG_FROM_IMAGE;
				command.params[0] = CommandArgument(cntxt->windowInfo.x0[segX]);
				command.params[1] = CommandArgument(cntxt->windowInfo.y0[segY]);
				command.params[2] = CommandArgument(cntxt->windowInfo.x1[segX] - cntxt->windowInfo.x0[segX]);
				command.params[3] = CommandArgument(cntxt->windowInfo.y1[segY] - cntxt->windowInfo.y0[segY]);
				command.params[4] = CommandArgument(iSeg);
				cntxt->synchro.pushInstr(&command);
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
				command.instr = NMC1_COPY_SEG_TO_IMAGE;
				cntxt->synchro.pushInstr(&command);
			}
		}
	}
//#endif // USED_POLYGONS_BUFFER

}
