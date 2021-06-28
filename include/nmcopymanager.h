#pragma once
#include "ringbuffert.h"
#include "dma.h"

template<class T, size_t SIZE, size_t NUM_CHANNELS, typename D> class NM_MemCopyManager{
  HalRingBufferConnector<T, SIZE> channels[NUM_CHANNELS];
public:
  HalRingBufferConnector<T, SIZE>* currentChannel;			// текущий ДМА канал
  bool isWorking = false;

  bool init() {
	  isWorking = false;
	  for (int ch = 0; ch < NUM_CHANNELS; ch++) {
		  if (channels[ch].create() == 0)
			  return false;
	  }
	  return true;
  }

  void startNextTask() {
	  isWorking = true;
	  for (int i = 0; i < NUM_CHANNELS; i++) {
		  currentChannel = channels + i;
		  if (!currentChannel->isEmpty()) {
			  static_cast<D*>(this)->startFunc(currentChannel->ptrTail());
			  return;
		  }
	  }
	  isWorking = false;
  }

  HalRingBufferConnector<T, SIZE>& getChannel(int channel = NUM_CHANNELS - 1){
  	return channels[channel];
  }


  void add(T* task, int channel = NUM_CHANNELS - 1){
	  channels[channel].push(task, 1);
  }

  void startJob() {
	  if (isWorking) {            // Не отвлекай ,  уже работаю 
		  return;
	  }
	  startNextTask();
  }

	bool isChannelCompleted(int priority = NUM_CHANNELS - 1){
		return channels[priority].isEmpty();
	}

	bool isJobCompleted() {
		for (int i = 0; i < NUM_CHANNELS; i++) {
			if (!channels[i].isEmpty())
				return false;
		}
		return true;
	}

};

