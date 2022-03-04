#ifndef BOARDMC12101_H
#define BOARDMC12101_H
#include <exception>
#include <cstdio>
#include <iostream>
#include <string>
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
protected:
    int _boardIndex = -1;
    PL_Access *access[MC12101_COUNT_OF_CORES];
    PL_Access *io_access[MC12101_COUNT_OF_CORES];
    bool accessed[MC12101_COUNT_OF_CORES];
    bool io_accessed[MC12101_COUNT_OF_CORES];
    PL_Board *desc;

    bool is_opened;

public:
    FILE *file_log[MC12101_COUNT_OF_CORES];
    ostream *nm_cout[MC12101_COUNT_OF_CORES];
    ostream *nm_cerr[MC12101_COUNT_OF_CORES];
    istream *nm_cin[MC12101_COUNT_OF_CORES];
    IO_Service *io_services[MC12101_COUNT_OF_CORES];

    virtual ~BoardMC12101();

    const char *programNames[MC12101_COUNT_OF_CORES];

    virtual void open() = 0;
    virtual void close() = 0;
    virtual bool isOpened() const = 0;

    virtual void connectToCore(int core) = 0;
    virtual void disconnectFromCore(int core) = 0;

    virtual int sync(int value, int core = 0) = 0;

    virtual void readMemBlock(PL_Addr src, void* dst, int size32, int core = 0) = 0;
    virtual void writeMemBlock(void* src, PL_Addr dst, int size32, int core = 0) = 0;

    virtual void loadProgram(const char *filename, int core ) = 0;

    virtual void setIO(int core, ostream *_out, ostream *_err, istream *_in) = 0;
    virtual void setIO(int core, const char *outfilename) = 0;

    virtual void openIO(const char *filename, int core) = 0;
    virtual void closeIO(int core) = 0;

    virtual void flushIO(int core) = 0;

    virtual void reset() = 0;
};

class BoardMC12101Local : public BoardMC12101{
public:
    BoardMC12101Local(int index = 0);
    ~BoardMC12101Local() override ;

    static int getBoardCount();

    void open() override;
    void close() override;

    bool isOpened() const override;

    void connectToCore(int core) override;
    void disconnectFromCore(int core) override;

    int sync(int value, int core = 0) override;

    void readMemBlock(PL_Addr src, void* dst, int size32, int core = 0) override;
    void writeMemBlock(void* src, PL_Addr dst, int size32, int core = 0) override;

    void loadProgram(const char *filename, int core ) override;

    void setIO(int core, ostream *_out, ostream *_err, istream *_in) override;
    void setIO(int core, const char *outfilename) override;

    void openIO(const char *filename, int core) override;
    void closeIO(int core) override;

    void flushIO(int core) override;

    void reset() override;
};

class BoardMC12101Error: public std::exception{
public:
    string message;
    int error;
    BoardMC12101 *board;
    static const char* errors[6];

    BoardMC12101Error(BoardMC12101 *_board, const char *_message, int error = 1);

    // exception interface
public:
    virtual const char *what() const noexcept{
        return message.c_str();
    };
};

#endif // BOARDMC12101_H
