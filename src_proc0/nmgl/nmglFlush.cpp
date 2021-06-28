#include "demo3d_nm0.h"
#include "nmgl.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglFlush (){

//#ifdef USED_POLYGONS_BUFFER
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	int nSegments = cntxt->currentSegments->count;
	Rectangle* rectangles = cntxt->currentSegments->rectangles;

	for (int iSeg = 0; iSeg < nSegments; iSeg++) {
		PolygonsConnector *trian_connector = cntxt->triangleConnectors + iSeg;
		PolygonsConnector *line_connector = cntxt->lineConnectors + iSeg;
		PolygonsConnector *point_connector = cntxt->pointConnectors + iSeg;
		bool drawingCheck = trian_connector->ptrHead()->count > 0 ||
							line_connector->ptrHead()->count > 0 ||
							point_connector->ptrHead()->count > 0;


		CommandNm1 command;
		if (drawingCheck) {
			command.instr = NMC1_COPY_SEG_FROM_IMAGE;
			command.params[0] = CommandArgument(rectangles[iSeg].x);
			command.params[1] = CommandArgument(rectangles[iSeg].y);
			command.params[2] = CommandArgument(rectangles[iSeg].width);
			command.params[3] = CommandArgument(rectangles[iSeg].height);
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
//#endif // USED_POLYGONS_BUFFER

}
