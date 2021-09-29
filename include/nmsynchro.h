#ifndef __NMSYNCHRO_H_INCLUDED__
#define __NMSYNCHRO_H_INCLUDED__

#include <time.h>
#include "nmdef.h"
#include "ringbuffert.h"
#include "stdio.h"
#include "link.h"
#include "led.h"
#include "hal.h"

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
#define NMC1_FINISH					0xF00D0000

#define NMC1_SET_ACTIVE_TEXTURE 	0xF0800000
#define NMC1_BIND_ACTIVE_TEX_OBJECT	0xF0810000
#define NMC1_SET_MIPMAP_LVL_POINTER 0xF0820000
#define NMC1_SET_TEX_ENV_COLOR 		0xF0830000
#define NMC1_SET_TEX_ENV_MODE 		0xF0840000
#define NMC1_SET_TEX_PARAMI 		0xF0850000
#define NMC1_SET_WHF				0xF0860000
#define NMC1_SET_COLOR_PALETTE		0xF0870000
#define NMC1_TEXTURE2D				0xF0880000

#define NMC1_AND 					0xF1020000
#define NMC1_AND4 					0xF1030000
#define NMC1_OR 					0xF1040000
#define NMC1_FAST_INV_SQRT 			0xF1050000
#define NMC1_POINT_SIZE 			0xF1060000
#define NMC1_CNV_32S_8S 			0xF1070000
#define NMC1_EXIT 					0xFFFF0000
#define NMC1_SYNC 					0xFF000000

#define SYNCHRO_EXIT 1

enum StoredTypes {
	TYPE_NULL,
	TYPE_POINTER,
	TYPE_INT,
	TYPE_UNSIGNED_INT,
	TYPE_FLOAT,
	TYPE_BOOL
};
struct CommandArgument {
	union {
		void* p;
		int i;
		unsigned int ui;	
		float f;
		bool b; 
	};
	StoredTypes storedType = TYPE_NULL;

	CommandArgument() {
		storedType = TYPE_NULL;
	}
	CommandArgument(void* value) {
		p = value;
		storedType = TYPE_POINTER;
	}
	CommandArgument(unsigned int value) {
		ui = value;
		storedType = TYPE_UNSIGNED_INT;
	}
	CommandArgument(int value) {
		i = value;
		storedType = TYPE_INT;
	}
	CommandArgument(float value) {
		f = value;
		storedType = TYPE_FLOAT;
	}
	CommandArgument(bool value) {
		b = value;
		storedType = TYPE_BOOL;
	}
};

struct NM_Command{
	int instr;
	CommandArgument params[7];
};

#define PRIORITY_SIZE 256
//#define PRIORITY_SIZE 2


typedef HalRingBufferData<NM_Command, PRIORITY_SIZE> NMGLSynchroData;


struct NMGL_SynchroMasterRingBuffer {
private:
	HalRingBufferConnector<NM_Command, PRIORITY_SIZE> connector;
	int dummy;
public:

	void init(NMGLSynchroData* synchroData) {
		connector.init(synchroData);
	}

	inline void pushInstr(NM_Command *command){
		while (connector.isFull());
		NM_Command* commandRB = connector.ptrHead();
		commandRB->instr = command->instr;
		for (int i = 0; i < 7; i++) {
			commandRB->params[i] = command->params[i];
		}
		(*connector.pHead)++;
	}

	inline bool isEmpty() {
		return connector.isEmpty();
	}
};

class NMGL_SynchroSlaveRingBuffer {
private:
public:
	HalRingBufferConnector<NM_Command, PRIORITY_SIZE> connector;
	int dummy;
public:

	void init(NMGLSynchroData* synchroData) {
		connector.init(synchroData);
	}

	inline void popInstr(NM_Command *command) {
		connector.pop(command, 1);
		for (int i = 0; i < 7; i++) {
			if (command->params[i].storedType == TYPE_POINTER) {
				command->params[i].p = halMapAddrFrom(command->params[i].p, 0);
			}
		}
	}
};


#endif