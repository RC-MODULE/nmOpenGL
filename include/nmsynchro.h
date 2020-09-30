#ifndef __NMSYNCHRO_H_INCLUDED__
#define __NMSYNCHRO_H_INCLUDED__

#include <time.h>
#include "nmdef.h"
#include "ringbuffert.h"
#include "stdio.h"

#define NMC1_CLEAR 					0xF0010000
#define NMC1_DRAW_TRIANGLES 		0xF0020000
#define NMC1_DRAW_LINES 			0xF0030000
#define NMC1_DRAW_POINTS 			0xF0040000
#define NMC1_DEPTH 					0xF0050000
#define NMC1_SET_COLOR 				0xF0060000
#define NMC1_SET_DEPTH 				0xF0070000
#define NMC1_SWAP_BUFFER 			0xF0080000
#define NMC1_DEPTH_MASK 			0xF0090000
#define NMC1_DEPTH_FUNC 			0xF00A0000
#define NMC1_COPY_SEG_FROM_IMAGE 	0xF00B0000
#define NMC1_COPY_SEG_TO_IMAGE 		0xF00C0000

#define NMC1_SET_ACTIVE_TEXTURE 	0xF0800000
#define NMC1_BIND_ACTIVE_TEX_OBJECT	0xF0810000
#define NMC1_SET_MIPMAP_LVL_POINTER 0xF0820000
#define NMC1_SET_TEX_ENV_COLOR 		0xF0830000
#define NMC1_SET_TEX_ENV_MODE 		0xF0840000
#define NMC1_SET_TEX_PARAMI 		0xF0850000
#define NMC1_SET_WHF				0xF0860000

#define NMC1_AND 					0xF1020000
#define NMC1_AND4 					0xF1030000
#define NMC1_OR 					0xF1040000
#define NMC1_FAST_INV_SQRT 			0xF1050000
#define NMC1_POINT_SIZE 			0xF1060000
#define NMC1_CNV_32S_8S 			0xF1070000
#define NMC1_EXIT 					0xFFFF0000
#define NMC1_SYNC 					0xFF000000

#define SYNCHRO_EXIT 1

struct CommandNm1{
	int instr_nmc1;
	int params[7];
};

#define PRIORITY_SIZE 256
//#define PRIORITY_SIZE 1

typedef HalRingBufferData<CommandNm1, PRIORITY_SIZE> NMGLSynchroData;

class NMGLSynchro {
private:
	HalRingBufferConnector<CommandNm1, PRIORITY_SIZE> connector;
	int dummy;
public:
	int time;
	int counter;

	void init(NMGLSynchroData* synchroData) {
		connector.init(synchroData);
		counter = 0;
	}

	void writeInstr(int priority, 
		int instr, 
		int param0 = 0, 
		int param1 = 0, 
		int param2 = 0, 
		int param3 = 0, 
		int param4 = 0, 
		int param5 = 0) {
		while (connector.isFull());
		CommandNm1* command = connector.ptrHead();
		command->instr_nmc1 = instr;
		command->params[0] = param0;
		command->params[1] = param1;
		command->params[2] = param2;
		command->params[3] = param3;
		command->params[4] = param4;
		command->params[5] = param5;
		(*connector.pHead)++;
		//halLed((connector.getHead() & 0xF) | (connector.getTail() << 4));
	}

	inline bool isEmpty() {
		return connector.isEmpty();
	}

	inline void popInstr(CommandNm1 *command) {
		connector.pop(command, 1);
		//halLed((connector.getHead() & 0xF) | (connector.getTail() << 4));
	}
};



#endif