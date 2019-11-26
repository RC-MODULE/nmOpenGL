#ifndef __NMSYNCHRO_H_INCLUDED__
#define __NMSYNCHRO_H_INCLUDED__

#include <time.h>
#include "nmdef.h"
#include "ringbuffer.h"

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
	volatile int instr_nmc1;
	volatile bool done;
	volatile int params[6];
};

#define PRIORITY0_SIZE 16
#define PRIORITY1_SIZE 128

struct NMGLSynchroData {
	CommandNm1 priority0[PRIORITY0_SIZE];
	CommandNm1 priority1[PRIORITY1_SIZE];
};

inline int copyRisc(const void* src, void* dst, int size32) {
	int* srcInt = (int*)src;
	int* dstInt = (int*)dst;
	for (int i = 0; i < size32; i++) {
		dstInt[i] = srcInt[i];
	}
	return 0;
}

class NMGLSynchro {
private:
	HalRingBuffer priority0RB;
	HalRingBuffer priority1RB;
public:
	int time0;
	int time1;
	int counter0;
	int counter1;

	void init(NMGLSynchroData* synchroData) {
		halRingBufferInit(&priority0RB, synchroData->priority0, sizeof32(CommandNm1), PRIORITY0_SIZE, copyRisc, 0, 0);
		halRingBufferInit(&priority1RB, synchroData->priority1, sizeof32(CommandNm1), PRIORITY1_SIZE, copyRisc, 0, 0);
	}

	void writeInstr(int priority, 
		int instr, 
		int param0 = 0, 
		int param1 = 0, 
		int param2 = 0, 
		int param3 = 0, 
		int param4 = 0, 
		int param5 = 0) {
		HalRingBuffer* current;
		switch (priority)
		{
		case 0:
			current = &priority0RB;
			break;
		case 1:
			current = &priority1RB;
			break;
		default:
			return;
		}
		while (halRingBufferIsFull(current));
		CommandNm1* command = (CommandNm1*)halRingBufferHead(current);
		command->instr_nmc1 = instr;
		command->done = false;
		command->params[0] = param0;
		command->params[1] = param1;
		command->params[2] = param2;
		command->params[3] = param3;
		command->params[4] = param4;
		command->params[5] = param5;
		current->head++;
	}

	bool isEmpty() {
		return halRingBufferIsEmpty(&priority1RB) && halRingBufferIsEmpty(&priority0RB);
	}

	void readInstr(CommandNm1* dst) {
		HalRingBuffer* currentBuffer;
		if (halRingBufferIsEmpty(&priority0RB)) {
			currentBuffer = &priority1RB;
		}
		else {
			currentBuffer = &priority0RB;
		}
		halRingBufferPop(currentBuffer, dst, 1);
	}
};



#endif
