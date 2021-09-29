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
	//connector.incTail();
	//return;

	DataForNmpu1* poly = connector.ptrTail();

	getAddrPtrnsL(poly);
	nm32s* mulZ = (nm32s*)context->buffer0;
	nm32s* mulC = (nm32s*)context->buffer0;
	nm32s* zMaskBuffer = (nm32s*)context->buffer1;
	int count = poly->count;
	
	msdWaitDma(0);
	
	int point = 0;
	
	msdWaitDma(1);

	poly->count = 0;
	connector.incTail();

	drawPrimitives(context, count);
	return;
}
