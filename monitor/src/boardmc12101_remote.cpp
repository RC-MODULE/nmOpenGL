#include "boardmc12101_remote.h"
#include <sstream>
#include <stdio.h>
#include <string>
#include <cstring>
#define MC12101_SYNC_TIMEOUT 5000

#ifdef _WIN32
#include "windows.h"
#endif

#ifdef unix
#include <unistd.h>
#endif

void BoardMC12101Remote::open()
{
    if(is_opened){
        return;
    }
    if(!is_connected){
        throw BoardMC12101Error(this, "No connection");
    }
    if(RPL_GetBoardDesc(_boardIndex, &desc)){
        throw BoardMC12101Error(this, "Can't open board");
    }
    is_opened = true;
}

void BoardMC12101Remote::close()
{
    if(int error = RPL_CloseBoardDesc(desc)){
        throw BoardMC12101Error(this, "Can't close board", error);
    }
    is_opened = false;
}

bool BoardMC12101Remote::isOpened() const{
    return is_opened;
}
bool BoardMC12101Remote::isConnected() const{
    return is_connected;
}

void BoardMC12101Remote::connectToHost(const char* hostaddr, int port){
    int error = RPL_ConnectToHost(hostaddr, port);
    if( error != RPL_OK){
        throw BoardMC12101Error(nullptr, "Failed connecting to remote server", error);
    } else{
        is_connected = true;
    }
}
void BoardMC12101Remote::disconnectFromHost(){
    if(is_connected){
        RPL_DisconnectFromHost();
        is_connected = false;
    }
}

BoardMC12101Remote::BoardMC12101Remote(int boardIndex){
    _boardIndex = boardIndex;
    is_opened = false;
    is_connected = false;
    for(int i = 0; i < MC12101_COUNT_OF_CORES; i++){
        accessed[i] = false;
        io_accessed[i] = false;
        static_cast<BoardMC12101Remote *>(this)->setIO(i, &std::cout, &std::cerr, &std::cin);
    }

}

BoardMC12101Remote::~BoardMC12101Remote(){
    if(!is_opened){
        return;
    }
    for(int core = 0; core < MC12101_COUNT_OF_CORES; core++){
        if(accessed[core])
            if (int error = RPL_CloseAccess(access[core]))
                std::cerr << "Fail close access " << core << ": " << BoardMC12101Error::errors[error] << std::endl;
        if(io_accessed[core])
            if (int error = RPL_CloseAccess(io_access[core]))
                std::cerr << "Fail close io access " << core << ": " << BoardMC12101Error::errors[error] << std::endl;
    }
    if (int error = RPL_CloseBoardDesc(desc)){
        std::cerr << "Fail close board" << std::endl;
    }
}

int BoardMC12101Remote::getBoardCount(){
    unsigned int count = 0;
    if(int error = RPL_GetBoardCount(&count)){
        throw BoardMC12101Error(nullptr, "Can't open driver", error);
    }
    return count;
}

void BoardMC12101Remote::setTimeout(uint32_t time){
    if(int error = RPL_SetTimeout(time)){
        throw BoardMC12101Error(this, "Can't set timeout", error);
    }
}

void BoardMC12101Remote::connectToCore(int core){
    if(core >= MC12101_COUNT_OF_CORES){
        throw BoardMC12101Error(this, "Out of range cores");
    }
    if(!accessed[core]){
        if(int error = RPL_GetAccess(desc, core, access + core)){
            throw BoardMC12101Error(this, "Can't connect", error);
        }
        accessed[core] = true;
    }
}

void BoardMC12101Remote::disconnectFromCore(int core){
    if(core >= MC12101_COUNT_OF_CORES){
        throw BoardMC12101Error(this, "Out of range cores");
    }
    if(accessed[core]){
        if(int error = RPL_CloseAccess(access[core])){
            throw BoardMC12101Error(this, "Can't disconnect", error);
        }
        accessed[core] = false;
    }
}

int BoardMC12101Remote::sync(int value, int core){
    int ret = 0;
    if(int error = RPL_Sync(access[core], value, &ret)){
        throw BoardMC12101Error(this, "sync fail", error);
    }
    return ret;
}


void BoardMC12101Remote::readMemBlock(int src, void* dst, int size32, int core){
    if(int error = RPL_ReadMemBlock(access[core], static_cast<RPL_Word *>(dst), src, size32)){
        stringstream str;
        str << "Read mem block error: src=0x" << hex << src << ", dst=0x" << hex << dst << ", size32=" << size32;
        throw BoardMC12101Error(this, str.str().c_str(), error);
    }
}

void BoardMC12101Remote::writeMemBlock(void* src, int dst, int size32, int core){
    if(int error = RPL_WriteMemBlock(access[core], static_cast<RPL_Word *>(src), dst, size32)){
        stringstream str;
        str << "Write mem block error: src=0x" << hex << src << ", dst=0x" << hex << dst << ", size32=" << size32;
        throw BoardMC12101Error(this, str.str().c_str(), error);
    }
}

void BoardMC12101Remote::loadProgram(const char *filename, int core ){
    if(accessed[core]){
        if(int error = RPL_LoadProgramFile(access[core], filename)){
            stringstream str;
            str << "Load program " << filename << " failed";
            throw BoardMC12101Error(this, str.str().c_str(), error);
        }
    }
    strcpy(programNames[core], filename);
}

const char *BoardMC12101Remote::getProgramName(int core){
    return programNames[core];
}

void BoardMC12101Remote::setIO(int core, ostream *_out, ostream *_err, istream *_in){
    nm_cout[core] = _out;
    nm_cerr[core] = _err;
    nm_cin[core] = _in;
}

void BoardMC12101Remote::setIO(int core, const char *outfilename){
    strcpy(logFileNames[core], outfilename);
    /*file_log[core] = fopen(outfilename, "w+");
    if(file_log[core] == 0){
        throw BoardMC12101Error(this, "set IO error");
    }*/
}

void BoardMC12101Remote::openIO(const char *filename, int core){
    if(int error = RPL_GetAccess(desc, core, &io_access[core])){
        throw BoardMC12101Error(this, "Opening IO error", error);
    }
    file_log[core] = fopen(logFileNames[core], "w+");
    if(file_log[core] == 0){
        throw BoardMC12101Error("fopen error");
    }
    if(file_log[core]){
        //io_services[core] = new IO_Service(filename, io_access[core], file_log[core]);
    } else {
        //io_services[core] = new IO_Service(filename, io_access[core], NULL, 1, nm_cout[core], nm_cerr[core], nm_cin[core]);
    }
    io_accessed[core] = true;
}

void BoardMC12101Remote::flushIO(int core){
    if(file_log[core]){
        try{
            fflush(file_log[core]);
        } catch (...){
            std::cerr << "nm io error" << std::endl;
        }
    } else{
        nm_cout[core]->flush();
    }

}

void BoardMC12101Remote::closeIO(int core){
    if(file_log[core])
        fclose(file_log[core]);
    //delete io_services[core];
    RPL_CloseAccess(io_access[core]);
    io_accessed[core] = false;
}

void BoardMC12101Remote::reset(){
    if(int error = RPL_ResetBoard(desc)){
        throw BoardMC12101Error(this, "Reset board error", error);
    }
    if(int error = RPL_LoadInitCode(desc)){
        throw BoardMC12101Error(this, "Load init code error", error);
    }

}

