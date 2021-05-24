#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include <stdio.h>
#include "imagebuffer.h"
#include "cache.h"


SECTION(".text_demo3d") void NMGL_DrawPoints(NMGL_Context_NM1 *context, CommandNm1 *command) {
	PolygonsConnector *connector = (PolygonsConnector*)command->params[0].p;

	DataForNmpu1* poly = connector->ptrTail();

	getAddrPtrnsP(poly);

	COMMON_DRAW_TYPE* mulZ = (COMMON_DRAW_TYPE*)context->buffer0;
	COMMON_DRAW_TYPE* mulC = (COMMON_DRAW_TYPE*)context->buffer0;
	COMMON_DRAW_TYPE* zMaskBuffer = (COMMON_DRAW_TYPE*)context->buffer1;
	int count = poly->count;

	msdWaitDma(0);
	int point = 0;

	msdWaitDma(1);
	
	poly->count = 0;
	connector->incTail();

	drawPrimitives(context, count);
}
