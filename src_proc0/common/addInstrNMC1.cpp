#include "ringbuffer.h"
#include "nmsynchro.h"


void addInstrNMC1(HalRingBuffer* commandsRB, int instr, 
	int param0 = 0, 
	int param1 = 0, 
	int param2 = 0, 
	int param3 = 0, 
	int param4 = 0,
	int param5 = 0) {
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
