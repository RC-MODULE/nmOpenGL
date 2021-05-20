#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include "imagebuffer.h"
#include "cache.h"
#include "nmprofiler.h"

int totalSum(nm32s* pVec, int size);

SECTION(".text_demo3d") void NMGL_DrawTriangles(NMGL_Context_NM1 *context, CommandNm1 *command) {

	PolygonsArray *data = (PolygonsArray*)command->params[0].p;
	PolygonsConnector connector;
	connector.init(data);

	DataForNmpu1* poly = connector.ptrTail();
	//nmprofiler_enable();
	//PROFILER_SIZE(poly->count);	
	getAddrPtrnsT(poly);
	//nmprofiler_disable();

	int countTriangles = poly->count;

	baseAddrOffs_32s((nm32s*)context->smallColorBuff.mData, context->imageOffsets, context->imagePoints, poly->count);
	baseAddrOffs_32s((nm32s*)context->smallDepthBuff.mData, context->imageOffsets, context->zBuffPoints, poly->count);

	nmppsCopy_32s(poly->color, context->buffer0, 4 * poly->count);
	nmppsConvert_32s8s(context->buffer0, (nm8s*)context->valuesC, 4 * poly->count);
	nmppsCopy_32s(poly->z, context->valuesZ, poly->count);
	msdWaitDma(1);

	poly->count = 0;
	connector.incTail();

	drawPrimitives(context, countTriangles);
	return;
}
