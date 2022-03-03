#include "nmgl_host_init.h"
#include "printnmlog.h"
#include <QDebug>

NMGL_HostInit::NMGL_HostInit(BoardMC12101 *_board, Refresh *host_program, PrintNmLog *_logs[]){
    board = _board;
    program = host_program;
    logs[0] = _logs[0];
    logs[1] = _logs[1];
}

NMGL_HostInit::~NMGL_HostInit(){
    print_thread->is_run = false;
    print_thread->wait();
    delete print_thread;
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

        program->fb = (NMGL_Framebuffer *)board->sync(0, 0);
        qDebug() << "Framebuffer addr: " << hex << program->fb;
    }

    catch (BoardMC12101Error e){
        qCritical() << e.what() << ": error: " << e.details();
        exit(1);
    }
    catch (...){
        qCritical() << "WTF!!";
        exit(2);
    }

    program->run();
}


