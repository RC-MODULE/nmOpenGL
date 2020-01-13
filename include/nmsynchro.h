#ifndef __NMSYNCHRO_H_INCLUDED__
#define __NMSYNCHRO_H_INCLUDED__

#include <time.h>
#include "nmdef.h"
#include "ringbuffer.h"
#include "ringbuffert.h"
#include "stdio.h"

#define NMC1_DRAW_TRIANGLES 0xF0000000
#define NMC1_DRAW_TRIANGLES_TEST 0xF0F00000
#define NMC1_DRAW_LINES 0xF0010000
#define NMC1_CLEAR 0xF0020000
#define NMC1_DEPTH 0xF0050000
#define NMC1_SET_COLOR 0xF0060000
#define NMC1_SET_DEPTH 0xF0070000
#define NMC1_SWAP_BUFFER 0xF0080000
#define NMC1_CNV_32S_8S 0xF0090000
#define NMC1_DEPTH_MASK 0xF00A0000
#define NMC1_DEPTH_FUNC 0xF00B0000
#define NMC1_COPY_SEG_FROM_IMAGE 0xF1000000
#define NMC1_COPY_SEG_TO_IMAGE 0xF1010000
#define NMC1_AND 0xF1020000
#define NMC1_AND4 0xF1030000
#define NMC1_OR 0xF1040000
#define NMC1_EXIT 0xFF000000

#define SYNCHRO_EXIT 1

struct CommandNm1{
	int instr_nmc1;
	int priority;
	int params[6];
};

#define PRIORITY0_SIZE 16
#define PRIORITY1_SIZE 128

struct NMGLSynchroData {

	HalRingBufferData<CommandNm1, PRIORITY0_SIZE> priority0;
	HalRingBufferData<CommandNm1, PRIORITY1_SIZE> priority1;

	void init() {
		priority0.init();
		priority1.init();
	}
};

class NMGLSynchro {
private:
	NMGLSynchroData* mSynchroData;
	int dummy;
	HalRingBufferConnector<CommandNm1, PRIORITY0_SIZE> connector0;
	HalRingBufferConnector<CommandNm1, PRIORITY1_SIZE> connector1;
public:
	int time;
	int counter;

	void init(NMGLSynchroData* synchroData) {
		mSynchroData = synchroData;
		connector0.init(&synchroData->priority0);
		connector1.init(&synchroData->priority1);
	}

	void writeInstr(int priority, 
		int instr, 
		int param0 = 0, 
		int param1 = 0, 
		int param2 = 0, 
		int param3 = 0, 
		int param4 = 0, 
		int param5 = 0) {
		
		CommandNm1* command;
		switch (priority)
		{
		case 0:
			while (connector0.isFull());
			command = connector0.ptrHead();
			break;
		case 1:
			while (connector1.isFull());
			command = connector1.ptrHead();
			break;
		default:
			return;
			break;
		}
		command->instr_nmc1 = instr;
		command->priority = priority;
		command->params[0] = param0;
		command->params[1] = param1;
		command->params[2] = param2;
		command->params[3] = param3;
		command->params[4] = param4;
		command->params[5] = param5;
		switch (priority)
		{
		case 0:
			(*connector0.pHead)++;
			break;
		case 1:
			(*connector1.pHead)++;
			break;
		}

	}

	inline bool isEmpty() {
		return isEmpty(1) && isEmpty(0);
	}

	bool isEmpty(int priority) {
		switch (priority)
		{
		case 0: {
			return connector0.isEmpty();
		}
		case 1: {
			return connector1.isEmpty();
		}
		}
	}

	void popInstr(CommandNm1 *command) {
		while (isEmpty());
		if (isEmpty(0)) {
			connector1.pop(command, 1);
		}
		else {
			connector0.pop(command, 1);
		}
	}
	
	unsigned int &getHead(int priority) {
		switch (priority)
		{
		case 0: {
			return *connector0.pHead;
		}
		case 1: {
			return *connector1.pHead;
		}
		}
	}

	unsigned int &getTail(int priority) {
		switch (priority)
		{
		case 0: {
			return *connector0.pTail;
		}
		case 1: {
			return *connector1.pTail;
		}
		}
	}
};



#endif
