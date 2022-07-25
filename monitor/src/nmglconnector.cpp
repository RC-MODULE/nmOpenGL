#include "nmglconnector.h"
#include <QDebug>
#include <QThread>

static BoardMC12101 *board;

void* pushFunc(const void *src, void *dst, unsigned int size){
    board->writeMemBlock(src, reinterpret_cast<PL_Addr>(dst), size);
}

void* popFunc(const void *src, void *dst, unsigned int size){
    board->readMemBlock(reinterpret_cast<PL_Addr>(src), dst, size);
}

void NMGL_Connector::checkCommandConnector(){
    if(!isInitialized) waitForInitialization();
    if(commandConnector == nullptr){
        PL_Addr addr;
        mBoard->readMemBlock(mRemote + mAddrOff + mType, &addr, mType);
        commandBufferDataAddr = reinterpret_cast<HalRingBufferData<NM_Command, HOST_COMMANDS_SIZE>*>(addr);

        commandConnector = new HalRingBufferConnector<NM_Command, HOST_COMMANDS_SIZE>(commandBufferDataAddr, pushFunc, popFunc);
        commandConnector->init(commandBufferDataAddr, pushFunc, popFunc);
    }
}

bool NMGL_Connector::connect(PL_Addr remote){
    if(mBoard){
        board = mBoard;
        mRemote = remote;
        int header = 0;
        mBoard->readMemBlock(mRemote, &header, 1);
        if (header != 0x971f2b10){
            std::cerr << "Wrong header nmgl data" << std::endl;
            return false;
        }

        mBoard->readMemBlock(mRemote + 1, &mType, 1);
        mBoard->readMemBlock(mRemote + 2, &mSizeOfStruct, 1);
        mBoard->readMemBlock(mRemote + 3, &mAddrOff, 1);

        isValid = true;
        return true;
    }
    return isValid;
}

void  NMGL_Connector::disconnect(){
    if(commandConnector) {
        delete commandConnector;
        commandConnector = nullptr;
    }
}

void NMGL_Connector::setProfile(bool status){
    if(isInitialized){
        checkCommandConnector();
        int data[4];
        data[0] = 1;
        data[1] = sizeof(CommandArgument);
        data[2] = status;
        while(commandConnector->isFull()){
            QThread::msleep(2);
        }
        commandConnector->memcopyPush(data, commandConnector->ptrHead(), 4);
        commandConnector->incHead();
    }
}

NMGL_Framebuffer *NMGL_Connector::framebuffer(){
    if(!isInitialized) waitForInitialization();
    int result = 0;
    mBoard->readMemBlock(mRemote + mAddrOff, &result, mType);
    return reinterpret_cast<NMGL_Framebuffer *>(result);
}

