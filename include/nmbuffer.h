#pragma once


/**
 * \brief      Простая реализация динамических буферов
 */
template <class T>
class SimpleBuffer{
	T* mBasicAddr;
	T* mTop;
	int mMaxSize;
	int mRemainSize;
public:
	void init(T* buffer, int maxSize){
		mBasicAddr = buffer;
		mTop = buffer;
		mMaxSize = maxSize;
		mRemainSize = mMaxSize;
	}
	T* top(){
		return mTop;
	}

	T* base(){
		return mBasicAddr;
	}

	T *alloc(int size){
		T* result = mTop;
		mTop += size;
		mRemainSize -= size;
		return result;
	}
	void bufferFree(int size){
		mTop -= size;
		mRemainSize += size;
	}

	void reset(){
		init(mBasicAddr, mMaxSize);
	}

	bool isCorrect(){
		return mRemainSize >= 0;
	}

};