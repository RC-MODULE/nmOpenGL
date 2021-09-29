#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include <stdio.h>
#include "imagebuffer.h"
#include "cache.h"


SECTION(".text_demo3d") void NMGL_DrawPoints(NMGL_Context_NM1 *context, NM_Command *command) {
	PolygonsConnector connector;
	connector.init((PolygonsArray*)command->params[0].p);
	DataForNmpu1* poly = connector.ptrTail();

	connector.incTail();
	return;


	DataForNmpu1* dataTmp = (DataForNmpu1*)context->buffers[0].alloc(sizeof32(DataForNmpu1));
	nmppsCopy_32s((nm32s*)poly, (nm32s*)dataTmp, 7 * POLYGONS_SIZE);
	dataTmp->count = poly->count;
	getAddrPtrnsP(dataTmp);
	context->buffers[0].free(sizeof32(DataForNmpu1));

	nm32s* mulZ = (nm32s*)context->buffer0;
	nm32s* mulC = (nm32s*)context->buffer0;
	nm32s* zMaskBuffer = (nm32s*)context->buffer1;
	int count = poly->count;

	msdWaitDma(0);
	msdWaitDma(1);
	
	poly->count = 0;
	connector.incTail();

	drawPrimitives(context, count);
}
