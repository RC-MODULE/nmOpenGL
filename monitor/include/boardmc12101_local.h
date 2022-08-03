#ifndef BOARDMC12101_LOCAL_H
#define BOARDMC12101_LOCAL_H
#include <exception>
#include <cstdio>
#include <iostream>
#include <string>
#include "mc12101load.h"
#include "boardmc12101.h"
#include <map>
#include "io_host.h"
/*
#ifdef unix
#include "io_host_lin.h"
#endif

#ifdef _WIN32
#include "io_host_win.h"
#endif*/


using namespace std;

#define MC12101_COUNT_OF_CORES 2

class BoardMC12101CoreLocal;
class BoardMC12101Local;






class BoardMC12101Local : public BoardMC12101{
private:

    char programNames[MC12101_COUNT_OF_CORES][1024];
    int _boardIndex = -1;
    bool is_opened;
    PL_Access *access[MC12101_COUNT_OF_CORES];
    PL_Access *io_access[MC12101_COUNT_OF_CORES];

    bool accessed[MC12101_COUNT_OF_CORES];
    bool io_accessed[MC12101_COUNT_OF_CORES];
    PL_Board *desc;


    ostream *nm_cout[MC12101_COUNT_OF_CORES];
    ostream *nm_cerr[MC12101_COUNT_OF_CORES];
    istream *nm_cin[MC12101_COUNT_OF_CORES];


    IO_Service *io_services[MC12101_COUNT_OF_CORES];

    std::map<int, BoardMC12101CoreLocal*> cores;
    char logFileNames[MC12101_COUNT_OF_CORES][1024];
    FILE *file_log[MC12101_COUNT_OF_CORES];
public:
    BoardMC12101Local(int index = 0);
    ~BoardMC12101Local() override ;

    static int getBoardCount();

    void open() override;
    void close() override;
    void reset() override;

    bool isOpened() const override;

    void connectToCore(int core) override;
    void disconnectFromCore(int core) override;

    int sync(int value, int core = 0) override;
    void setTimeout(uint32_t time) override;

    void readMemBlock(int src, void* dst, int size32, int core = 0) override;
    void writeMemBlock(void* src, int dst, int size32, int core = 0) override;

    void loadProgram(const char *filename, int core ) override;
    const char*getProgramName(int core) override;

    void setIO(int core, ostream *_out, ostream *_err, istream *_in) override;
    void setIO(int core, const char *outfilename) override;

    void openIO(const char *filename, int core) override;
    void closeIO(int core) override;

    void flushIO(int core) override;

};

class BoardMC12101IO_Local : public BoardMC12101IO{
private:
    BoardMC12101Local *mBoard;
    PL_Board *mDescriptor;
    PL_Access *mAccess;
    char programName[512];
    char outputFileName[512];
    // BoardMC12101IO interface
    PL_Access *getAccess(){
        /*if(mAccess == nullptr){
            if(int error = PL_GetAccess(mDescriptor, core, &mAccess)){
                throw BoardMC12101Error("Failed get access for io");
            }
        }*/
        return mAccess;
    }

public:
    BoardMC12101IO_Local(){
        mBoard = nullptr;
        mAccess = nullptr;
    }
    void setBoard(BoardMC12101 *board) override{
        BoardMC12101Local *localBoard = dynamic_cast<BoardMC12101Local *>(board);
        if(localBoard){
            mBoard = localBoard;
        }
    }
    void setOut(int core, const char *outfilename) override{
    }
    void setProgram(int core, const char *programAbsFile) override{

    }
    bool open() override{
        return false;
    }
    void close() override{

    }
    void flush() override{

    }
};


/*class BoardMC12101CoreLocal : public BoardMC12101Core{
private:
    PL_Access *mAccess;
    PL_Board *mBoard;
    char programName[1024];
    BoardMC12101Local *board;
public:
    explicit BoardMC12101CoreLocal(PL_Board *boardDesc);

    void connect() override;
    void disconnect() override;

    int sync(int value) override;

    void readMemBlock(PL_Addr src, void* dst, int size32) override;
    void writeMemBlock(void* src, PL_Addr dst, int size32) override;

    void loadProgram(const char *filename) override;
    const char *getFileName() override;
    void setTimeout(uint32_t time) override;

    BoardMC12101 *getBoard() override;

};*/


#endif // BOARDMC12101_LOCAL_H
