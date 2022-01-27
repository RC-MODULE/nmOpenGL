#pragma once
#include "demo3d_common.h"
#include "demo3d_nm0.h"
#include "nmtype.h"
#include "nmblas.h"
#include "nmglservice_nm0.h"

extern int contextIsModified;

SECTION(".text_demo3d") void NMGL_SetValue(NM_Command &command){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    cntxt->synchro.pushInstr(&command);
}

SECTION(".text_demo3d") NM_Command NMGL_GetValue(int mode){
	//cntxt->synchro.pushInstr(&command);	
	NM_Command command;
	return command;
}

SECTION(".text_demo3d") void NMGL_PushSegment(Rectangle &rect, int segNo){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    NM_Command command;
    command.instr = NMC1_COPY_SEG_TO_IMAGE;
    command.params[0] = CommandArgument(rect.x);
    command.params[1] = CommandArgument(rect.y);
    command.params[2] = CommandArgument(rect.width);
    command.params[3] = CommandArgument(rect.height);
    command.params[4] = CommandArgument(segNo);
    cntxt->synchro.pushInstr(&command);
}

SECTION(".text_demo3d") void NMGL_PopSegment(Rectangle &rect, int segNo){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    NM_Command command;
    command.instr = NMC1_COPY_SEG_FROM_IMAGE;
    command.params[0] = CommandArgument(rect.x);
    command.params[1] = CommandArgument(rect.y);
    command.params[2] = CommandArgument(rect.width);
    command.params[3] = CommandArgument(rect.height);
    command.params[4] = CommandArgument(segNo);
    cntxt->synchro.pushInstr(&command);
}

DataForNmpu1* NMGL_PolygonsCurrent(int mode, int segNo){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    switch(mode){
    case NMGL_TRIANGLES:    
		return cntxt->triangleConnectors[segNo].ptrHead();
    case NMGL_LINES:    
		return cntxt->lineConnectors[segNo].ptrHead();
    case NMGL_POINTS:    
		return cntxt->pointConnectors[segNo].ptrHead();
    default:
        return 0;
    }
}


void NMGL_PolygonsMoveNext(int mode, int segNo){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    PolygonsConnector *connector;
	NM_Command command;
    switch(mode){
    case NMGL_TRIANGLES:    
        connector = cntxt->triangleConnectors + segNo;
		command.instr = NMC1_DRAW_TRIANGLES;
        break;
    case NMGL_LINES:    
        connector = cntxt->lineConnectors + segNo;
		command.instr = NMC1_DRAW_LINES;
        break;
    case NMGL_POINTS:    
        connector = cntxt->pointConnectors + segNo;
		command.instr = NMC1_DRAW_POINTS;
        break;
    default:
        return;
    }
    command.params[0] = CommandArgument(connector->ringbufferDataPointer);
    command.params[1] = CommandArgument(segNo);
    cntxt->synchro.pushInstr(&command);
    connector->incHead();
    while (connector->isFull()) {
        halSleep(2);
    }
}


