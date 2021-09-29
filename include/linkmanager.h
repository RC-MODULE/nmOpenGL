#pragma once

#include "link.h"
#include "ringbuffert.h"

#define LINK_1D 0
#define LINK_2D 1
#define LINK_SIZE 16

struct LinkTask {
	void* data;
	int size32 = 0;
	int width;
	int stride;
	int type = LINK_1D;
	LinkCallback callback = 0;
};

template<int PORT, int DIRECTION>
class NM_MemCopyManagerLink : public NM_MemCopyManager<LinkTask, LINK_SIZE, 1, NM_MemCopyManagerLink<PORT, DIRECTION>> {
public:
#ifdef __GNUC__
	void startFunc(LinkTask* task) {
		if (task->type == LINK_1D) {
			halLinkStart(task->data, task->size32, PORT, DIRECTION);
		}
		else {
			halLinkStart2D(task->data, task->size32, task->width, task->stride, PORT, DIRECTION);
		}
	}
#else
	void startFunc(LinkTask* task) {
		
	}
#endif
};
