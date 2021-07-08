#ifndef __MY_SERVER_DMA_H__
#define __MY_SERVER_DMA_H__ 
#include "time.h"
#include "dma.h"
#include "ringbuffert.h"

#define MSD_DMA 1
#define MSD_DMA_2D 2
#define MSD_SIZE 256
#define MSD_NUM_CHANNELS 3

typedef int(*msdCallback)();

/*!
 *  \brief Структура для работы с DMA
 *  
 */
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
	if (dmaCopy->type == MSD_DMA) {
		halDmaStartA(dmaCopy->src, dmaCopy->dst, dmaCopy->size);
	}
	else {
		halDma2D_StartA(dmaCopy->src, dmaCopy->dst,
			dmaCopy->size, dmaCopy->width,
			dmaCopy->srcStride, dmaCopy->dstStride);
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


/*!
 *  \brief Сервер работы с DMA
 */
template <int BUFFER_SIZE, int NUM_CHANNELS> class MyDmaServer {
	msdCallback originCallback;												// адрес преустановленного обработчика DMA   
public:
	bool	isWorking;														// флаг сервер находится в режиме исполнения DMA задач
	HalRingBufferConnector<MyDmaTask, BUFFER_SIZE>  channel[NUM_CHANNELS];		// DMA каналы 
	HalRingBufferConnector<MyDmaTask, BUFFER_SIZE>* currentChannel;			// текущий ДМА канал
	MyDmaTask* currentTask;													// Указатель на текущую задчу. По нему в callback функции можно , например, выставить статус завершенной
																			// Иниализация кольцевых буферов DMA задач
																			// Иниализация DMA , сохранение обработчика ПДП
	void init(msdCallback dmaCallback) {
		
		isWorking = false;
		for (int ch = 0; ch < NUM_CHANNELS; ch++) {
			channel[ch].create();											// выделяем кольцевой буфер задач
		}
		halDmaInit();														// инициализация ПДП (halDmaInit halDmaInitC halDmaInitA halDmaInitM ... )
																			//originCallback=halDmaGetCallback();									// сохраняем текущий обработчик ПДП
		halDmaSetCallback(dmaCallback);								// назначаем новый callback (обработчик ПДП)
	}

	~MyDmaServer() {
		halDmaSetCallback(originCallback);									// Восстановлиям исходный обработчик ПДП
	}

	// возвращает адрес кольцевого буфера канала для присоединения DMA клиентом 
	HalRingBufferData<MyDmaTask, BUFFER_SIZE>* getChannelBuffer(int idxChannel) {
		return channel[idxChannel].container;
	}
	// возвращает статус , что все задачи во всех каналах выполнены
	inline bool isJobCompleted() {
		for (int i = 0; i < NUM_CHANNELS; i++) {
			if (!isChannelCompleted(i))
				return false;
		}
		return true;
	}

	// возвращает статус , что все задачи в данном канале выполнены
	inline bool isChannelCompleted(int ch) {
		return channel[ch].isEmpty();
	}


	// здесь установливается порядок обхода задач, (например, стратегия где задачи выибираются по одной поочередно из каждого канала. В данном примере важно чтобы NUM_CHANNELS - степерь двойка
	inline void startNextTask() {
#ifdef __GNUC__
		isWorking = true;					// флаг что сервер работает
		for (int i = 0; i < NUM_CHANNELS; i++) {
			currentChannel = &channel[i];
			if (!currentChannel->isEmpty()) {
				msdStartCopy(currentChannel->ptrTail());
				return;
			}
		}
		isWorking = false;				// флаг что сервер стоит
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
	// Запустить все задачи на на исполнение 
	void startJob() {
		if (isWorking) {						// Не отвелекай ,  уже работаю 
			return;
		}
		startNextTask();
	}

};

/*!
 *  \brief Клиент сервера DMA
 */
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