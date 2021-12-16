#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include <nmpp.h>
#include "linkmanager.h"
#include "link.h"


SECTION(".data_demo3d") NM_MemCopyManagerLink<0, LINK_INPUT> linkIn;
SECTION(".data_demo3d") NM_MemCopyManagerLink<1, LINK_OUTPUT> linkOut;

SECTION(".text_demo3d") void lmCallbackIn() {
	halLedOn(6);
	LinkCallback callback = linkIn.currentChannel->ptrTail()->callback;
	if (callback != 0) {
		callback();
	}
	linkIn.currentChannel->incTail();
	linkIn.startNextTask();
	halLedOff(6);
}
SECTION(".text_demo3d") void lmCallbackOut() {
	halLedOn(7);
	LinkCallback callback = linkOut.currentChannel->ptrTail()->callback;
	if (callback != 0) {
		callback();
	}
	linkOut.currentChannel->incTail();
	linkOut.startNextTask();
	halLedOff(7);
}

SECTION(".text_demo3d") void lmInit() {
#ifdef __GNUC__
	halLinkInit(0, LINK_INPUT);
	halLinkInit(1, LINK_OUTPUT);
	int mode = 0x3;
	*((int*)0x40000006) = 0x33;

	halLinkSetCallback(lmCallbackIn, 0, LINK_INPUT);
	halLinkSetCallback(lmCallbackOut, 1, LINK_OUTPUT);
#endif
	int heapNo = getHeap();
	setHeap(10);
	if (!linkIn.init()) {
		printf("linkIn error\n");
	}
	if (!linkOut.init()) {
		printf("linkOut error\n");
	}
	setHeap(heapNo);
}

SECTION(".text_demo3d") void lmAddReceive(LinkTask &task) {
	linkIn.add(&task);
	linkIn.startJob();
	//while (linkIn.isJobCompleted() == 0);
}

SECTION(".text_demo3d") void lmAddTransfer(LinkTask &task) {
	linkOut.add(&task);
	linkOut.startJob();
	//while (linkOut.isJobCompleted() == 0);
}

SECTION(".text_demo3d") void lmWaitReceive() {
	while(linkIn.isJobCompleted() == 0);
}

SECTION(".text_demo3d") void lmWaitTransfer() {
	while(linkOut.isJobCompleted() == 0);
}



