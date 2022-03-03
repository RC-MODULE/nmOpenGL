#include "hostprogram.h"
#include "printnmlog.h"
#include <QDebug>

void HostProgram::init(){
    try {
        const char *program_name[2];
        program_name[0] = "main0d.abs";
        program_name[1] = "main1d.abs";
        qDebug() << "Program name 0: " << program_name[0];
        qDebug() << "Program name 1: " << program_name[1];
        m_board->loadProgram(program_name[0], 0);
        m_board->loadProgram(program_name[1], 1);
        qDebug() << "program loaded";

        //print_thread = new PrintNmLogThread(board, logs);
        //print_thread->start();

        int handshake = m_board->sync(0xC0DE0086, 0);
        if (handshake != 0xC0DE0000) {
            qCritical() << "Error: Handshake with mc12101-nmc0 wrong!";
            exit(handshake);
        }
        qDebug() << "Handshake passed";

        fb = (NMGL_Framebuffer *)m_board->sync(0, 0);
        qDebug() << "Framebuffer addr: " << hex << fb;
    }

    catch (BoardMC12101Error e){
        qCritical() << e.what() << ": error: " << e.details();
        exit(1);
    }
    catch (...){
        qCritical() << "WTF!!";
        exit(2);
    }
}


