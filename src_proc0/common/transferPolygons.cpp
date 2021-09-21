/*!
 *  \file copyColorByIndices_BGRA_RGBA.cpp
 *  \author Жиленков Иван
 */
#include "demo3d_common.h"
#include "demo3d_nm0.h"
#include "nmtype.h"


SECTION(".text_demo3d")
void transferPolygons(PolygonsConnector *connector, int mode, int segNo) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NM_Command command;
	command.instr = mode;
	command.params[0] = CommandArgument(connector->ringbufferDataPointer);
	command.params[1] = CommandArgument(segNo);
	cntxt->synchro.pushInstr(&command);
}
