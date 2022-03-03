#ifndef BOARDMC12101_H
#define BOARDMC12101_H
#include <exception>
#include <cstdio>
#include <iostream>
#include "mc12101load.h"

#ifdef unix
#include "io_host_lin.h"
#endif

#ifdef _WIN32
#include "io_host_win.h"
#endif


using namespace std;

#define MC12101_COUNT_OF_CORES 2

class BoardMC12101
{
private:
    int _boardIndex = -1;
    PL_Access *access[MC12101_COUNT_OF_CORES];
    PL_Access *access_io[MC12101_COUNT_OF_CORES];
    bool accessed[MC12101_COUNT_OF_CORES];
    PL_Board *desc;


public:
    FILE *file_log[MC12101_COUNT_OF_CORES];
    ostream *nm_cout[MC12101_COUNT_OF_CORES];
    ostream *nm_cerr[MC12101_COUNT_OF_CORES];
    istream *nm_cin[MC12101_COUNT_OF_CORES];
    IO_Service *io_services[MC12101_COUNT_OF_CORES];

    const char *program_name[MC12101_COUNT_OF_CORES];

    BoardMC12101(int index = 0);
    ~BoardMC12101();

    static int getBoardCount();

    void open(int board);
    void close();

    void connectToCore(int core);
    void disconnectFromCore(int core);

    int sync(int value, int core = 0);

    PL_Access *getAccess(int core);

    void readMemBlock(PL_Addr src, void* dst, int size32, int core = 0);
    void writeMemBlock(void* src, PL_Addr dst, int size32, int core = 0);

    void loadProgram(const char *filename, int core );

    void setIO(int core, ostream *_out, ostream *_err, istream *_in);
    void setIO(int core, const char *outfilename);

    void openIO(const char *filename, int core);
    void closeIO(int core);

    void flushIO(int core);
};

class BoardMC12101Error: public exception{
public:
    const char *message;
    int error;
    BoardMC12101 *board;
    const char* errors[6] = {"OK", "ERROR", "TIMEOUT", "FILE", "BADADDRESS", "NOT_IMPLEMENTED"};

    BoardMC12101Error(BoardMC12101 *_board, const char *_message, int error = 1);

    // exception interface
public:
    virtual const char *what() const noexcept{
        return message;
    };
    const char *details() const noexcept{
        return errors[error];
    };
};

#endif // BOARDMC12101_H
