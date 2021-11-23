#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include <stdio.h>
#include "imagebuffer.h"
#include "cache.h"


SECTION(".text_demo3d") void NMGL_DrawLines(NMGL_Context_NM1 *context, NM_Command *command) {
	PolygonsArray* polygons = (PolygonsArray*)command->params[0].p;
	PolygonsConnector connector; 
	connector.init(polygons);

	DataForNmpu1* poly = connector.ptrTail();

	getAddrPtrnsL(poly);
	
	msdWaitDma(0);
	msdWaitDma(1);

	int count = poly->count;
	poly->count = 0;
	connector.incTail();

	drawPrimitives(context, count);
	return;
}
