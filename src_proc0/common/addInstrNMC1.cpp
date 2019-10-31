#include "ringbuffer.h"
#include "nmsynchro.h"
#include "demo3d_common.h"
//#include "stdio.h"
//#include "nmgl_data0.h"

SECTION(".text_demo3d")
void addInstrNMC1(HalRingBuffer* commandsRB, int instr, 
	int param0, 
	int param1, 
	int param2, 
	int param3, 
	int param4,
	int param5) {

	//printf("head-tail=%d\n", cntxt.synchro->commandsRB.head - cntxt.synchro->commandsRB.tail);
	volatile int a = 0;
	a++;
	while (halRingBufferIsFull(commandsRB)) {
		a++;
	}
	
	CommandNm1* command = (CommandNm1*)halRingBufferHead(commandsRB);
	command->params[0] = param0;
	command->params[1] = param1;
	command->params[2] = param2;
	command->params[3] = param3;
	command->params[4] = param4;
	command->params[5] = param5;
	command->instr_nmc1 = instr;
	command->done = false;

	commandsRB->head++;
}
