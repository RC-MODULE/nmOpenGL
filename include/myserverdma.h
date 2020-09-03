#ifndef __MY_SERVER_DMA_H__
#define __MY_SERVER_DMA_H__ 

#define MSD_DMA 1
#define MSD_DMA_2D 2
#define MSD_SIZE 256
#define MSD_NUM_CHANNELS 3

typedef int(*msdCallback)();

struct MyDmaTask {
	const void* src;
	void* dst;
	int size;
	int width;
	int srcStride;
	int dstStride;
	int type;
	msdCallback callback;
};

#ifdef __GNUC__
inline void msdStartCopy(MyDmaTask* dmaCopy) {
	switch (dmaCopy->type)
	{
	case MSD_DMA:
		halDmaStartA(dmaCopy->src, dmaCopy->dst, dmaCopy->size);
		break;
	case MSD_DMA_2D:
		halDma2D_StartA(dmaCopy->src, dmaCopy->dst,
			dmaCopy->size, dmaCopy->width,
			dmaCopy->srcStride, dmaCopy->dstStride);
		break;
	default:
		break;
	}
}
#else
inline void msdStartCopy(MyDmaTask* dmaCopy) {
	switch (dmaCopy->type)
	{
	case MSD_DMA:
		halCopyRISC(dmaCopy->src, dmaCopy->dst, dmaCopy->size);
		break;
	case MSD_DMA_2D:
		for (int y = 0; y < dmaCopy->size / dmaCopy->width; y++) {
			int* src = (int*)dmaCopy->src + y * dmaCopy->srcStride;
			int* dst = (int*)dmaCopy->dst + y * dmaCopy->dstStride;
			halCopyRISC(src, dst, dmaCopy->width);
		}
		break;
	default:
		break;
	}
}
#endif // __GNUC__



template <int BUFFER_SIZE, int NUM_CHANNELS> class MyDmaServer {
	msdCallback originCallback;												// ����� ����������������� ����������� DMA   
public:
	bool	isWorking;														// ���� ������ ��������� � ������ ���������� DMA �����
	HalRingBufferConnector<MyDmaTask, BUFFER_SIZE>  channel[NUM_CHANNELS];		// DMA ������ 
	HalRingBufferConnector<MyDmaTask, BUFFER_SIZE>* currentChannel;			// ������� ��� �����
	MyDmaTask* currentTask;													// ��������� �� ������� �����. �� ���� � callback ������� ����� , ��������, ��������� ������ �����������
																			// ����������� ��������� ������� DMA �����
																			// ����������� DMA , ���������� ����������� ���
	void init(msdCallback dmaCallback) {
		
		isWorking = false;
		for (int ch = 0; ch < NUM_CHANNELS; ch++) {
			channel[ch].create();											// �������� ��������� ����� �����
		}
		halDmaInit();														// ������������� ��� (halDmaInit halDmaInitC halDmaInitA halDmaInitM ... )
																			//originCallback=halDmaGetCallback();									// ��������� ������� ���������� ���
		halDmaSetCallback(dmaCallback);								// ��������� ����� callback (���������� ���)
	}

	~MyDmaServer() {
		halDmaSetCallback(originCallback);									// ������������� �������� ���������� ���
	}

	// ���������� ����� ���������� ������ ������ ��� ������������� DMA �������� 
	HalRingBufferData<MyDmaTask, BUFFER_SIZE>* getChannelBuffer(int idxChannel) {
		return channel[idxChannel].container;
	}
	// ���������� ������ , ��� ��� ������ �� ���� ������� ���������
	inline bool isJobCompleted() {
		for (int i = 0; i < NUM_CHANNELS; i++) {
			if (!isChannelCompleted(i))
				return false;
		}
		return true;
	}

	// ���������� ������ , ��� ��� ������ � ������ ������ ���������
	inline bool isChannelCompleted(int ch) {
		return channel[ch].isEmpty();
	}


	// ����� ��������������� ������� ������ �����, (��������, ��������� ��� ������ ����������� �� ����� ���������� �� ������� ������. � ������ ������� ����� ����� NUM_CHANNELS - ������� ������
	inline void startNextTask() {
#ifdef __NM__
		isWorking = true;					// ���� ��� ������ ������
		for (int i = 0; i < NUM_CHANNELS; i++) {
			currentChannel = &channel[i];
			if (!currentChannel->isEmpty()) {
				msdStartCopy(currentChannel->ptrTail());
				return;
			}
		}
		isWorking = false;				// ���� ��� ������ ����
#else
		for (int i = 0; i < NUM_CHANNELS; i++) {
			currentChannel = &channel[i];
			if (!currentChannel->isEmpty()) {
				MyDmaTask* currentTask = currentChannel->ptrTail();
				msdStartCopy(currentTask);
				if (currentTask->callback != 0) {
					currentTask->callback();
				}
				currentChannel->incTail();
				return;
			}
		}
#endif
	}
	// ��������� ��� ������ �� �� ���������� 
	void startJob() {
		if (isWorking) {						// �� ��������� ,  ��� ������� 
			return;
		}
		startNextTask();
	}

};


template <int BUFFER_SIZE>	class MyDmaClient : public HalRingBufferConnector<MyDmaTask, BUFFER_SIZE> {
public:

	inline void bind(void* channelBuffer) {
		HalRingBufferConnector<MyDmaTask, BUFFER_SIZE>::connect(channelBuffer);
	}
	inline void add(MyDmaTask& task) {
		HalRingBufferConnector<MyDmaTask, BUFFER_SIZE>::push(&task, 1);
	}
	void run() {

	}
};

void msdInit();
unsigned int msdAdd(const void* src, void* dst, int size, int priority = MSD_NUM_CHANNELS - 1);
unsigned int msdAdd2D(const void* src, void* dst, unsigned size, unsigned width, unsigned srcStride32, unsigned dstStride32, int priority = MSD_NUM_CHANNELS - 1);
void msdWaitDma();
void msdWaitDma(int channel);
bool msdGetStatusCopy(int index, int priority = MSD_NUM_CHANNELS - 1);

unsigned int msdAdd(MyDmaTask &task, int priority = MSD_NUM_CHANNELS - 1);


#endif