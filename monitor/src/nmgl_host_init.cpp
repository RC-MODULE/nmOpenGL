#include "nmgl_host_init.h"
#include "printnmlog.h"
#include <QDebug>

NMGL_HostInit::NMGL_HostInit(BoardMC12101 *_board, Refresh *_refresh, PrintNmLog *_logs[]){
    board = _board;
    refresh = _refresh;
    logs[0] = _logs[0];
    logs[1] = _logs[1];
}

void NMGL_HostInit::run(){
    try {
        const char *program_name[2];
        program_name[0] = "main0d.abs";
        program_name[1] = "main1d.abs";
        qDebug() << "Program name 0: " << program_name[0];
        qDebug() << "Program name 1: " << program_name[1];
        board->loadProgram(program_name[0], 0);
        board->loadProgram(program_name[1], 1);
        qDebug() << "program loaded";

        print_thread = new PrintNmLogThread(board, logs);
        print_thread->start();

        int handshake = board->sync(0xC0DE0086, 0);
        if (handshake != 0xC0DE0000) {
            qCritical() << "Error: Handshake with mc12101-nmc0 wrong!";
            exit(handshake);
        }
        qDebug() << "Handshake passed";

        refresh->fb = (NMGL_Framebuffer *)board->sync(0, 0);
        qDebug() << "Framebuffer addr: 0x" << hex << refresh->fb << endl;
    }

    catch (BoardMC12101Error e){
        qCritical() << e.what();
        exit(0);
    }
    catch (...){
        qCritical() << "WTF!!";
        exit(0);
    }

    refresh->start();
}


