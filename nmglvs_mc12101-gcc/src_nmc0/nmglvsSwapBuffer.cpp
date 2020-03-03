#include "demo3d_nm0.h"
#include "nmgl.h"
#include "hal.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

//extern Polygons polygons[36];

void nmglvsSwapBuffer(){
	static int time = 0;
	static clock_t t0, t1;
	t1 = clock();
	cntxt.synchro.counter++;
	cntxt.synchro.time = t1-t0;
	t0 = clock();

	/*PolygonsConnector connector(cntxt.polygonsData);

	for (int segY = 0, iSeg = 0; segY < cntxt.windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt.windowInfo.nColumns; segX++, iSeg++) {
			Polygons* poly = polygons + iSeg;

			if (poly->count != 0) {
				cntxt.synchro.writeInstr(1, NMC1_COPY_SEG_FROM_IMAGE,
					cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y0[segY],
					cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY],
					iSeg);

				nmblas_scopy(sizeof32(Polygons), (float*)poly, 1, (float*)connector.ptrHead(), 1);
				poly->count = 0;
				connector.incHead();
				cntxt.synchro.writeInstr(1, NMC1_DRAW_TRIANGLES);

				cntxt.synchro.writeInstr(1,
					NMC1_COPY_SEG_TO_IMAGE,
					cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y0[segY],
					cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY]);
			}
		}
	}*/

	cntxt.synchro.writeInstr(1, NMC1_SWAP_BUFFER, 0, 0, 0, 0, 0, 0);
	
}