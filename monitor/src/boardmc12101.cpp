#include "boardmc12101.h"
#include "mc12101load.h"
#define MC12101_SYNC_TIMEOUT 5000

void BoardMC12101::open(int board)
{
    if(_boardIndex != -1){
        for(int i = 0; i < MC12101_COUNT_OF_CORES; i++){
            disconnectFromCore(i);
            closeIO(i);
        }
        close();
    }
    _boardIndex = board;
    if(PL_GetBoardDesc(_boardIndex, &desc)){
        throw BoardMC12101Error(this, "Can't open board");
    }
}

void BoardMC12101::close()
{
    if(PL_CloseBoardDesc(desc)){
        std::cerr << "Can't close board" << std::endl;
    }
}

BoardMC12101::BoardMC12101(int boardIndex){
    open(boardIndex);
    PL_ResetBoard(desc);


    for(int i = 0; i < MC12101_COUNT_OF_CORES; i++){
        accessed[i] = false;
        setIO(i, &std::cout, &std::cerr, &std::cin);
    }

    //PL_SetTimeout(MC12101_SYNC_TIMEOUT);
}

BoardMC12101::~BoardMC12101(){
    for(int core = 0; core < MC12101_COUNT_OF_CORES; core++){
        try{
            //delete io_services[core];
            disconnectFromCore(core);
            closeIO(core);
        } catch(BoardMC12101Error e){
            std::cerr << e.what() << endl;
        }
    }
    close();
}

int BoardMC12101::getBoardCount(){
    unsigned int count = 0;
    if(PL_GetBoardCount(&count)){
        throw BoardMC12101Error(nullptr, "Can't open driver");
    }
    return count;
}

void BoardMC12101::connectToCore(int core){
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

void BoardMC12101::disconnectFromCore(int core){
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

int BoardMC12101::sync(int value, int core){
    int ret = 0;
    if(int error = PL_Sync(access[core], value, &ret)){
        throw BoardMC12101Error(this, "sync fail", error);
    }
    return ret;
}

PL_Access *BoardMC12101::getAccess(int core){
    return access[core];
}

void BoardMC12101::readMemBlock(PL_Addr src, void* dst, int size32, int core){
    if(int error = PL_ReadMemBlock(access[core], static_cast<PL_Word *>(dst), src, size32)){
        throw BoardMC12101Error(this, "LoadProgram failed", error);
    }
}

void BoardMC12101::writeMemBlock(void* src, PL_Addr dst, int size32, int core){
    if(int error = PL_WriteMemBlock(access[core], static_cast<PL_Word *>(src), dst, size32)){
        throw BoardMC12101Error(this, "LoadProgram failed", error);
    }
}

void BoardMC12101::loadProgram(const char *filename, int core ){
    if(accessed[core]){
        if(int error = PL_LoadProgramFile(access[core], filename)){
            throw BoardMC12101Error(this, "LoadProgram failed", error);
        }
        program_name[core] = filename;
    }

}

void BoardMC12101::setIO(int core, ostream *_out, ostream *_err, istream *_in){
    nm_cout[core] = _out;
    nm_cerr[core] = _err;
    nm_cin[core] = _in;
}
void BoardMC12101::setIO(int core, const char *outfilename){
    file_log[core] = fopen(outfilename, "w+");
    if(file_log[core] == 0){
        throw BoardMC12101Error(this, "set IO error");
    }
}

void BoardMC12101::openIO(const char *filename, int core){
    PL_GetAccess(desc, core, &access_io[core]);
    if(file_log[core]){
        io_services[core] = new IO_Service(filename, access_io[core], file_log[core]);
    } else {
        io_services[core] = new IO_Service(filename, access_io[core], NULL, 1, nm_cout[core], nm_cerr[core], nm_cin[core]);
    }
}

void BoardMC12101::flushIO(int core){
    if(file_log[core] == 0)
        return;
    io_services[core]->dispatch();
    //delete io_services[core];
    //fclose(file_log[core]);
    //fopen(file_log[core]);
    //io_services[core] = new IO_Service(program_name[core], access_io[core], file_log[core]);
}

void BoardMC12101::closeIO(int core){
    if(file_log[core])
        fclose(file_log[core]);
    delete io_services[core];
    PL_CloseAccess(access_io[core]);
}

BoardMC12101Error::BoardMC12101Error(BoardMC12101 *_board, const char *_message, int _error){
    board = _board;
    message = _message;
    error = _error;
}

//const char *BoardMC12101Error::what()const {
//    return message;
//}
