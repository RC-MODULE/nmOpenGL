#ifndef NMGL_HOST_INIT_H
#define NMGL_HOST_INIT_H
#include <QThread>
#include "boardmc12101.h"
#include "printnmlog.h"
#include "framebuffer.h"
#include "refresh.h"

class NMGL_HostInit : public QThread{
public:
    NMGL_HostInit(BoardMC12101 *_board, Refresh *host_program, PrintNmLog *_logs[]);
    ~NMGL_HostInit();

    BoardMC12101 *board = 0;

    Refresh *program;

    PrintNmLog *logs[2];
    PrintNmLogThread *print_thread;

protected:
    void run();

};

#endif //NMGL_INIT_H

