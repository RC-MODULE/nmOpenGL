#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"
#include "debugprint.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglFlush (){
	DEBUG_PLOG_FUNCTION();
//#ifdef USED_POLYGONS_BUFFER
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	int nSegments = cntxt->currentSegments->count;
	Rectangle* rectangles = cntxt->currentSegments->rectangles;

	for (int iSeg = 0; iSeg < nSegments; iSeg++) {
		bool drawingCheck = NMGL_PolygonsCurrent(NMGL_TRIANGLES, iSeg)->count > 0 ||
			NMGL_PolygonsCurrent(NMGL_LINES, iSeg)->count > 0 ||
			NMGL_PolygonsCurrent(NMGL_POINTS, iSeg)->count > 0;


		if (drawingCheck) {
			NMGL_PopSegment(rectangles[iSeg], iSeg);

			if (NMGL_PolygonsCurrent(NMGL_TRIANGLES, iSeg)->count) {
				NMGL_PolygonsMoveNext(NMGL_TRIANGLES, iSeg);
				NMGL_PolygonsCurrent(NMGL_TRIANGLES, iSeg)->count = 0;
			}
			if (NMGL_PolygonsCurrent(NMGL_LINES, iSeg)->count) {
				NMGL_PolygonsMoveNext(NMGL_LINES, iSeg);
				NMGL_PolygonsCurrent(NMGL_LINES, iSeg)->count = 0;
			}
			if (NMGL_PolygonsCurrent(NMGL_POINTS, iSeg)->count) {
				NMGL_PolygonsMoveNext(NMGL_POINTS, iSeg);
				NMGL_PolygonsCurrent(NMGL_POINTS, iSeg)->count = 0;
			}

			NMGL_PushSegment(rectangles[iSeg], iSeg);
		}
		
	}
//#endif // USED_POLYGONS_BUFFER

}
