#include "hal_target.h"
#include "hal.h"
#include <time.h>
#include "stdio.h"
#include "nmpp.h"
#include "nmgl.h"
#include "nmglvs_nmc0.h"
#include "nmprofiler.h"
#include "nmsynchro.h"

SECTION(".data_imu0") NMGL_Framebuffer defaultFramebuffer;
SECTION(".data_imu0") NM_Command segInfo;
SECTION(".data_imu2") int temp[128*128];

SECTION(".text_shared0") int main()
{
#if defined(__GNUC__) && defined(DEBUG)
	//nmprofiler_init();
	//nmprofiler_disable();
#endif // __GNUC__
	//halLedOn(0);
	halLinkInit(1, LINK_INPUT);
	halLinkInit(1, LINK_OUTPUT);

	int fromHost = halHostSync(0xC0DE0000);		// send handshake to host
	if (fromHost != 0xC0DE0086) {					// get  handshake from host
		return 1;
	}

	setHeap(10);
	
	defaultFramebuffer.imageBufferFront.data = halMalloc32(WIDTH_IMAGE * HEIGHT_IMAGE);
	setHeap(12);
	defaultFramebuffer.imageBuffer.data = halMalloc32(WIDTH_IMAGE * HEIGHT_IMAGE);
	defaultFramebuffer.setSize(WIDTH_IMAGE, HEIGHT_IMAGE);
	for (int i = 0; i < defaultFramebuffer.getSize(); i++) {
		((int*)defaultFramebuffer.imageBufferFront.data)[i] = 0;
		((int*)defaultFramebuffer.imageBuffer.data)[i] = 0;
	}

	halHostSyncAddr(&defaultFramebuffer);
	//halLedOn(1);

	while (true) {
		halLinkStart(&segInfo, sizeof32(NM_Command), 1, LINK_INPUT);
		while (halLinkIsCompleted(1, LINK_INPUT) == 0);

		switch (segInfo.instr) {
			case NMC1_COPY_SEG_TO_IMAGE:{
				int size = segInfo.params[1].ui;
				halLinkStart(temp, size, 1, LINK_INPUT);
				while (halLinkIsCompleted(1, LINK_INPUT) == 0);
				halCopyRISC(temp, (int*)defaultFramebuffer.imageBuffer.data + segInfo.params[0].ui, size);
				break;
			}
			case NMC1_SWAP_BUFFER: {
				while (defaultFramebuffer.tail == defaultFramebuffer.head - 1);
				void *frontData = defaultFramebuffer.imageBufferFront.data;
				defaultFramebuffer.imageBufferFront.data = defaultFramebuffer.imageBuffer.data;
				defaultFramebuffer.imageBuffer.data = frontData;
				defaultFramebuffer.head++;
				break;
			}
		}

		
	}


	return 0x600D600D;
} 
