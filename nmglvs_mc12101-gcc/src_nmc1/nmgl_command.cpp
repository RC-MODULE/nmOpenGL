#include "demo3d_nm1.h"
#include "hal.h"

SECTION(".data_imu0") NM_Command currentCommand;

extern NMGL_SynchroSlaveRingBuffer nmglSynchro;

SECTION(".text_demo3d") NM_Command* NMGL_ReadNM_Command(){
	halLedOn(4);
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	nmglSynchro.popInstr(&currentCommand);
	halLedOff(4);
	return &currentCommand;
}
