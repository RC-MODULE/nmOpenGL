//------------------------------------------------------------------------
//  Demostration for MAX 2017 
//
//  Author: Alexander Bolornikov
//
//  Copyright (c) 2017 RC Module Inc.
//------------------------------------------------------------------------

#include "VShell.h"
#include "hal.h"
#include "hal_host.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "nmpp.h"
#include "demo3d_host.h"
#include "demo3d_nm1.h"
#include "ringbuffer_host.h"
#include <thread>

#include "phillips.h"
#include "hostprofiler.h"
#include "nmglvs_host.h"



using namespace std;

#define		FRAME_WIDTH				256
#define		FRAME_HEIGHT			256
#define		FRAME_SIZE		    	FRAME_WIDTH * FRAME_HEIGHT

int currentImage[WIDTH_IMAGE * HEIGHT_IMAGE];

int main()
{
	if (nmglvsHostInit() != 0) {
		return 1;
	}
	int ok = 0;
	nm32f* omega = nmppsMalloc_32f(FRAME_SIZE);

	nm32fcr* h0 = nmppsMalloc_32fcr(FRAME_SIZE);
	nm32fcr* h0Conjugated = nmppsMalloc_32fcr(FRAME_SIZE);
	
	unsigned omegaAddrInNm = halSync(0xF00D1);

	unsigned h0AddrInNm = halSync(0xF00D2);
	unsigned h0ConjugatedAddrInNm = halSync(0xF00D3);

	computeH0(h0, h0Conjugated, FRAME_WIDTH, FRAME_HEIGHT);
	computeOmega(omega, FRAME_WIDTH, FRAME_HEIGHT);

	ok = halWriteMemBlock((unsigned*)omega, (unsigned)omegaAddrInNm, FRAME_SIZE);
	ok = halWriteMemBlock((unsigned*)h0, (unsigned)h0AddrInNm, 2 * FRAME_SIZE);
	ok = halWriteMemBlock((unsigned*)h0Conjugated, (unsigned)h0ConjugatedAddrInNm, 2 * FRAME_SIZE);

	ok = halSync(0x600D600D);
	unsigned error = halSync(0xF00D3);
	//----------------init-VShell--------------------------------------------
	if (!VS_Init())
		return 0;
	VS_CreateImage("Source Image", 1, WIDTH_IMAGE, HEIGHT_IMAGE, VS_RGB32, 0);	// Create window for 8-bit source grayscale image
	VS_OpRunForward();


	while(VS_Run())	{
		nmglvsHostReadImage(currentImage);

		VS_SetData(1, currentImage);

		VS_Draw(VS_DRAW_ALL);
	}

	unsigned int result[2];
	nmglvsExit(result);
	return 0;
};
