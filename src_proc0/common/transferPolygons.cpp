/*!
 *  \file copyColorByIndices_BGRA_RGBA.cpp
 *  \author Жиленков Иван
 */
#include "demo3d_common.h"
#include "demo3d_nm0.h"
#include "nmtype.h"


SECTION(".text_demo3d")
void transferPolygons(DataForNmpu1 *data, PolygonsConnector *connector, int mode) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	connector->incHead();
	while (connector->isFull()) {
		halSleep(2);
	}
	connector->ptrHead()->count = 0;
	cntxt->synchro.writeInstr(1, mode, (int)connector + 0x40000);
}
