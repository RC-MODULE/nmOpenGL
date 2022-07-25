#ifndef __NMGL_REMOTE_CONNECTOR_INCLUDED__
#define __NMGL_REMOTE_CONNECTOR_INCLUDED__
#include "nmglremote.h"
#include "boardmc12101.h"
#include <stddef.h>
#include "ringbuffert.h"

class NMGL_Connector{
    PL_Addr mRemote;
    int mHead;
    int mTail;
    int mType;
    int mAddrOff;
    int mSizeOfStruct;
    BoardMC12101 *mBoard = NULL;
    bool isValid = false;
    HalRingBufferData<NM_Command, HOST_COMMANDS_SIZE> *commandBufferDataAddr = nullptr;
    HalRingBufferConnector<NM_Command, HOST_COMMANDS_SIZE> *commandConnector = nullptr;

    PL_Addr mProfilerHeadAddr;
    bool isInitialized = false;

    void checkCommandConnector();
public:

    bool connect(PL_Addr remote);

    void waitForInitialization(){
        while(initializingProcess() < 1);
    }

    void disconnect();

    void setProfile(bool status);

    void setBoard(BoardMC12101 *board){
        mBoard = board;
    }

    NMGL_Framebuffer *framebuffer();

    PL_Addr profilerHeadAddr() const{
        checkCommandConnector();

        size_t offset = offsetof(NMGL_RemoteAccess, nmprofiler_head_addr) / 4;
        mBoard->readMemBlock(mRemote + offset, &mProfilerHeadAddr, 1);
        return mProfilerHeadAddr;
    }

    unsigned long long head(){
        if(!isValid) return 0;
        size_t offset = offsetof(NMGL_RemoteAccess, head) / 4;
        mBoard->readMemBlock(mRemote + offset, &mHead, 2);
        return mHead;
    }

    unsigned long long tail(){
        if(!isValid) return 0;
        size_t offset = offsetof(NMGL_RemoteAccess, tail) / 4;
        mBoard->readMemBlock(mRemote + offset, &mTail, 2);
        return mTail;
    }

    float initializingProcess(){
        if(!isValid) return 0;
        float result;
        size_t offset = offsetof(NMGL_RemoteAccess, initializingProcess) / 4;
        mBoard->readMemBlock(mRemote + offset, &result, 1);
        isInitialized = result >= 1;
        return result;
    }

};

#endif //__NMGL_REMOTE_CONNECTOR_INCLUDED__
