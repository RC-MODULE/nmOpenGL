#include "boardmc12101.h"
#include "mc12101load.h"
#include <sstream>
#include <stdio.h>
#include <cstring>
#define MC12101_SYNC_TIMEOUT 5000

#ifdef _WIN32
#include "windows.h"
#endif

#ifdef unix
#include <unistd.h>
#endif

const char* BoardMC12101Error::errors[6] = {"OK", "ERROR", "TIMEOUT", "FILE", "BADADDRESS", "NOT_IMPLEMENTED"};

void BoardMC12101Local::open()
{
    if(is_opened){
        return;
    }
    if(PL_GetBoardDesc(_boardIndex, &desc)){
        throw BoardMC12101Error(this, "Can't open board");
    }
    is_opened = true;
}

void BoardMC12101Local::close()
{
    if(int error = PL_CloseBoardDesc(desc)){
        throw BoardMC12101Error(this, "Can't close board", error);
    }
    is_opened = false;
}

bool BoardMC12101Local::isOpened() const{
    return is_opened;
}

BoardMC12101Local::BoardMC12101Local(int boardIndex){
    _boardIndex = boardIndex;
    is_opened = false;
    for(int i = 0; i < MC12101_COUNT_OF_CORES; i++){
        accessed[i] = false;
        io_accessed[i] = false;
        static_cast<BoardMC12101Local *>(this)->setIO(i, &std::cout, &std::cerr, &std::cin);
    }
    //PL_SetTimeout(MC12101_SYNC_TIMEOUT);
}

BoardMC12101Local::~BoardMC12101Local(){
    if(!is_opened){
        return;
    }
    for(int core = 0; core < MC12101_COUNT_OF_CORES; core++){
        if(accessed[core])
            if (int error = PL_CloseAccess(access[core]))
                std::cerr << "Fail close access " << core << ": " << BoardMC12101Error::errors[error] << std::endl;
        if(io_accessed[core])
            if (int error = PL_CloseAccess(io_access[core]))
                std::cerr << "Fail close io access " << core << ": " << BoardMC12101Error::errors[error] << std::endl;
    }
    if (int error = PL_CloseBoardDesc(desc)){
        std::cerr << "Fail close board" << std::endl;
    }
}

int BoardMC12101Local::getBoardCount(){
    unsigned int count = 0;
    if(PL_GetBoardCount(&count)){
        throw BoardMC12101Error(nullptr, "Can't open driver");
    }
    return count;
}

void BoardMC12101Local::connectToCore(int core){
    if(core >= MC12101_COUNT_OF_CORES){
        throw BoardMC12101Error(this, "Out of range cores");
    }
    if(!accessed[core]){
        if(int error = PL_GetAccess(desc, core, access + core)){
            throw BoardMC12101Error(this, "Can't connect", error);
        }
        accessed[core] = true;
    }
}

void BoardMC12101Local::disconnectFromCore(int core){
    if(core >= MC12101_COUNT_OF_CORES){
        throw BoardMC12101Error(this, "Out of range cores");
    }
    if(accessed[core]){
        if(int error = PL_CloseAccess(access[core])){
            throw BoardMC12101Error(this, "Can't disconnect", error);
        }
        accessed[core] = false;
    }
}

int BoardMC12101Local::sync(int value, int core){
    int ret = 0;
    if(int error = PL_Sync(access[core], value, &ret)){
        throw BoardMC12101Error(this, "sync fail", error);
    }
    return ret;
}


void BoardMC12101Local::readMemBlock(PL_Addr src, void* dst, int size32, int core){
    if(int error = PL_ReadMemBlock(access[core], static_cast<PL_Word *>(dst), src, size32)){
        throw BoardMC12101Error(this, "LoadProgram failed", error);
    }
}

void BoardMC12101Local::writeMemBlock(void* src, PL_Addr dst, int size32, int core){
    if(int error = PL_WriteMemBlock(access[core], static_cast<PL_Word *>(src), dst, size32)){
        throw BoardMC12101Error(this, "LoadProgram failed", error);
    }
}

void BoardMC12101Local::loadProgram(const char *filename, int core ){
    if(accessed[core]){
        if(int error = PL_LoadProgramFile(access[core], filename)){
            throw BoardMC12101Error(this, "LoadProgram failed", error);
        }
    }
    strcpy(programNames[core], filename);

}

void BoardMC12101Local::setIO(int core, ostream *_out, ostream *_err, istream *_in){
    nm_cout[core] = _out;
    nm_cerr[core] = _err;
    nm_cin[core] = _in;
}

void BoardMC12101Local::setIO(int core, const char *outfilename){
    file_log[core] = fopen(outfilename, "w+");
    if(file_log[core] == 0){
        throw BoardMC12101Error(this, "set IO error");
    }
}

void BoardMC12101Local::openIO(const char *filename, int core){
    if(int error = PL_GetAccess(desc, core, &io_access[core])){
        throw BoardMC12101Error(this, "Opening IO error", error);
    }
    if(file_log[core]){
        io_services[core] = new IO_Service(filename, io_access[core], file_log[core]);
    } else {
        //io_services[core] = new IO_Service(filename, io_access[core], NULL, 1, nm_cout[core], nm_cerr[core], nm_cin[core]);
        io_services[core] = new IO_Service(filename, io_access[core], NULL);
    }
    io_accessed[core] = true;
}

void BoardMC12101Local::flushIO(int core){
    if(file_log[core]){
        try{
            //closeIO(core);
            //openIO(programNames[core], core);
            //io_services[core]->dispatch();
            //delete io_services[core];
            //io_services[core] = new IO_Service(programNames[core], io_access[core], file_log[core]);
        } catch (...){
            std::cerr << "nm io error" << std::endl;
        }
    } else{
        nm_cout[core]->flush();
    }

}

void BoardMC12101Local::closeIO(int core){
    if(file_log[core])
        fclose(file_log[core]);
    delete io_services[core];
    PL_CloseAccess(io_access[core]);
    io_accessed[core] = false;
}

void BoardMC12101Local::reset(){
    if(int error = PL_ResetBoard(desc)){
        throw BoardMC12101Error(this, "Reset board error", error);
    }
}

BoardMC12101Error::BoardMC12101Error(BoardMC12101 *_board, const char *_message, int _error)
{
    stringstream mes;
    board = _board;
    mes << _message << ": " << errors[_error];
    message = mes.str();
    error = _error;
}

BoardMC12101::~BoardMC12101(){

}

BoardMC12101Access::BoardMC12101Access(BoardMC12101 *board, int core){
    mBoard = board;
    mCore = core;
}

BoardMC12101Access::~BoardMC12101Access(){

}
