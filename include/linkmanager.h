#pragma once

#include "link.h"
#include "ringbuffert.h"

#define LINK_1D 0
#define LINK_2D 1
#define LINK_SIZE 16

struct LinkTask {
	void* data;
	int size32;
	int width;
	int stride;
	int type;
	LinkCallback callback = 0;
};

template<int PORT, int DIRECTION>
class NM_MemCopyManagerLink : public NM_MemCopyManager<LinkTask, MSD_SIZE, MSD_NUM_CHANNELS, NM_MemCopyManagerLink<PORT, DIRECTION>> {
public:
	void startFunc(LinkTask* task) {
		if (task->type == MSD_DMA) {
			halLinkStart(task->data, task->size32, PORT, DIRECTION);
		}
		else {
			halLinkStart2D(task->data, task->size32, task->width, task->stride, PORT, DIRECTION);
		}
	}
};
