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

struct Synchro{
	volatile int counter_nmc0;
	volatile int counter_nmc1;
	volatile int exit_nm;
	volatile int hasInstrHost;
	volatile unsigned int time0;
	volatile unsigned int time1;
	volatile int instrFromHost;
	volatile int fr;
	
	HalRingBuffer commandsRB;
	HalRingBuffer instantCommandsRB;

};



#endif
