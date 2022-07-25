#ifndef __NMGL_REMOTE_INCLUDED__
#define __NMGL_REMOTE_INCLUDED__
#include "framebuffer.h"
#include "ringbuffert.h"
#include "nmsynchro.h"

#define MAX_CONNECTED_COUNT 2
#define HOST_COMMANDS_SIZE 2


struct NMGL_RemoteAccess{
    NMGL_RemoteAccess();
    //порядок важен
    int header = 0x971f2b10;
    int type;       // 0 - incorrect, 1 - 32 bit, 2 - 64 bit
    int sizeOfStruct;    // Размер структуры
    int addrOff;    // Смещение до адресов
    volatile unsigned long long head;
    volatile unsigned long long tail[MAX_CONNECTED_COUNT];
    int connectedCounter = 0;
    int image_counter = 0;
    int nmprofiler_head_addr;
    float initializingProcess = 0.0f;  // progress engine initialization

    alignas(8) NMGL_Framebuffer *framebuffer;
    HalRingBufferData<NM_Command, HOST_COMMANDS_SIZE> *fromHostCommands;
};


void NMGL_RemoteAccessProcessEvents();
NMGL_RemoteAccess *NMGL_RemoteAccessInstance();


#endif //__NMGL_REMOTE_INCLUDED__
