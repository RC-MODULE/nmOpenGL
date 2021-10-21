#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include "imagebuffer.h"
#include "cache.h"
#include "nmprofiler.h"

int totalSum(nm32s* pVec, int size);

SECTION(".text_demo3d") void NMGL_DrawTriangles(NMGL_Context_NM1 *context, NM_Command *command) {

	PolygonsArray *data = (PolygonsArray*)command->params[0].p;
	PolygonsConnector connector;
	connector.init(data);


	DataForNmpu1* poly = connector.ptrTail();
	DataForNmpu1* dataTmp = (DataForNmpu1*)context->buffers[0].alloc(sizeof32(DataForNmpu1));
	nmppsCopy_32s((nm32s*)poly, (nm32s*)dataTmp, 7 * POLYGONS_SIZE);
	dataTmp->count = poly->count;
	getAddrPtrnsT(dataTmp);
	context->buffers[0].free(sizeof32(DataForNmpu1));
	//connector.incTail();
	//return;

	baseAddrOffs_32s((nm32s*)context->smallColorBuff.mData, context->imageOffsets, context->imagePoints, poly->count);
	baseAddrOffs_32s((nm32s*)context->smallDepthBuff.mData, context->imageOffsets, context->zBuffPoints, poly->count);

	nmppsCopy_32s(poly->color, context->buffer0, 4 * poly->count);
	nmppsConvert_32s8s(context->buffer0, (nm8s*)context->valuesC, 4 * poly->count);
	nmppsCopy_32s(poly->z, context->valuesZ, poly->count);
//TEXTURING_PART
	//for nmglShadeModel: || cntxt->shadeModel == NMGL_SMOOTH
	if (context->texState.textureEnabled) {
		for (int i = 0; i < poly->count; i++) {
			context->x0[i] = (float)poly->x0[i] + context->texState.segX0;
			context->y0[i] = (float)poly->y0[i] + context->texState.segY0;
			context->x1[i] = (float)poly->x1[i] + context->texState.segX0;
			context->y1[i] = (float)poly->y1[i] + context->texState.segY0;
			context->x2[i] = (float)poly->x2[i] + context->texState.segX0;
			context->y2[i] = (float)poly->y2[i] + context->texState.segY0;

			context->texS0[i] = poly->s0[i];
			context->texT0[i] = poly->t0[i];
			context->texS1[i] = poly->s1[i];
			context->texT1[i] = poly->t1[i];
			context->texS2[i] = poly->s2[i];
			context->texT2[i] = poly->t2[i];

			context->w0[i] = poly->w0[i];
			context->w1[i] = poly->w1[i];
			context->w2[i] = poly->w2[i];
		}
	}
//TEXTURING_PART
	msdWaitDma(1);

	int countTriangles = poly->count;
	poly->count = 0;
	connector.incTail();

	drawPrimitives(context, countTriangles);
	return;
}
