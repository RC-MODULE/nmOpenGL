#include "nmglremote.h"
#include "section-hal.h"
#include <stddef.h>
#include <nmprofiler.h>

INSECTION(".nmgl_remote") NMGL_RemoteAccess remoteAccess;

INSECTION(".nmgl_imu0") HalRingBufferData<NM_Command, HOST_COMMANDS_SIZE> fromHostCommands;

NMGL_RemoteAccess::NMGL_RemoteAccess() : type(1), addrOff(offsetof(NMGL_RemoteAccess, framebuffer)), sizeOfStruct(sizeof32(NMGL_RemoteAccess)){
    head = 0;
    for(int i = 0; i < MAX_CONNECTED_COUNT; i++){
        tail[i] = 0;
    }
    framebuffer = 0;
}

extern int nmprofiler_head_addr;

extern "C" {
    __attribute__((constructor)) void NMGL_RemoteAccessInit(){
        remoteAccess.fromHostCommands = &fromHostCommands;
        for(int i = 0; i < MAX_CONNECTED_COUNT; i++){
            remoteAccess.tail[i] = 0;
        }
        fromHostCommands.init();    
        
        nmprofiler_init();
        nmprofiler_disable();
        remoteAccess.nmprofiler_head_addr = nmprofiler_head_addr;
    }
}

NMGL_RemoteAccess *NMGL_RemoteAccessInstance(){
	return &remoteAccess;
}

void NMGL_RemoteAccessProcessEvents(){
    if(fromHostCommands.isEmpty()) return;

    NM_Command command;
    
	halCopyRISC(fromHostCommands.ptrTail(), &command, sizeof32(NM_Command));
    fromHostCommands.tail++;
    switch(command.instr){
        case 1:
        printf("status = %d\n", command.params[0].i);
        if(command.params[0].b){
            nmprofiler_enable();
            printf("profile enabled\n");
        } else{
            nmprofiler_disable();
            printf("profile disabled\n");
        }
        break;
    }
}

