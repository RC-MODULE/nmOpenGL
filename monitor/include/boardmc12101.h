#ifndef BOARDMC12101_H
#define BOARDMC12101_H
#include <exception>
#include <cstdio>
#include <iostream>
#include "mc12101load.h"


using namespace std;

#define MC12101_COUNT_OF_CORES 2

class BoardMC12101;
class BoardMC12101Core;
class BoardMC12101Error;



class BoardMC12101
{
public:

    virtual ~BoardMC12101();


    virtual void open() = 0;
    virtual void close() = 0;
    virtual void reset() = 0;
    virtual bool isOpened() const = 0;

    //virtual BoardMC12101Core *getCore(int core);

    virtual void connectToCore(int core) = 0;
    virtual void disconnectFromCore(int core) = 0;

    virtual int sync(int value, int core = 0) = 0;
    virtual void setTimeout(uint32_t time){};

    virtual void readMemBlock(PL_Addr src, void* dst, int size32, int core = 0) = 0;
    virtual void writeMemBlock(void* src, PL_Addr dst, int size32, int core = 0) = 0;

    virtual void loadProgram(const char *filename, int core ) = 0;
    virtual const char *getProgramName(int core) = 0;

    virtual void setIO(int core, ostream *_out, ostream *_err, istream *_in) = 0;
    virtual void setIO(int core, const char *outfilename) = 0;

    virtual void openIO(const char *filename, int core) = 0;
    virtual void closeIO(int core) = 0;

    virtual void flushIO(int core) = 0;

};

class BoardMC12101Core{
public:
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual int sync(int value) = 0;

    virtual void readMemBlock(PL_Addr src, void* dst, int size32) = 0;
    virtual void writeMemBlock(void* src, PL_Addr dst, int size32) = 0;

    virtual void loadProgram(const char *filename) = 0;
    virtual const char *getFileName() = 0;
    virtual void setTimeout(uint32_t time){};

    virtual BoardMC12101 *getBoard() = 0;
    virtual void setBoard(BoardMC12101 *board) = 0;
};

class BoardMC12101_IO{
public:
    virtual void setIO(ostream *_out, ostream *_err, istream *_in) = 0;
    virtual void setIO(int core, const char *outfilename) = 0;
    virtual void open(const char *filename) = 0;
    virtual void close() = 0;
};






class BoardMC12101Error: public std::exception{
public:
    string message;
    int error;
    BoardMC12101 *board;
    static const char* errors[6];

    BoardMC12101Error(BoardMC12101 *_board, const char *_message, int error = 1);
    BoardMC12101Error(const char *_message, int error = 1);

    // exception interface
public:
    virtual const char *what() const noexcept{
        return message.c_str();
    };
};

#endif // BOARDMC12101_H
