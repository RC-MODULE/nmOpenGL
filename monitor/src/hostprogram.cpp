#include "hostprogram.h"
#include "printnmlog.h"
#include <QDebug>

HostProgram::~HostProgram(){
    /*if(print_thread){
        print_thread->is_run = false;
        print_thread->wait();
    }
    delete print_thread;*/
}

void HostProgram::host_main(){
    while(is_run){
        if(fb == NULL) continue;

        while(frameBufferIsEmpty(fb)){
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        readColorFrontNM(imageTemp, fb, 0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);
        frameBufferIncTail(fb);
        for(int y = 0; y < HEIGHT_IMAGE; y++){
            for(int x = 0; x < WIDTH_IMAGE; x++){
                imageDraw[y * WIDTH_IMAGE + x] = imageTemp[(HEIGHT_IMAGE - y - 1) * WIDTH_IMAGE + x];
            }
        }
        QImage image((const uchar *)imageDraw, 768, 768, QImage::Format_RGB32);
        m_label->setPixmap(QPixmap::fromImage(image).scaled(m_label->size()));
        m_label->update();
    }
}

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
        if(!is_run) quit();

        int handshake = m_board->sync(0xC0DE0086, 0);
        if (handshake != 0xC0DE0000) {
            qCritical() << "Error: Handshake with mc12101-nmc0 wrong!";
            exit(handshake);
        }
        qDebug() << "Handshake passed";

        if(!is_run) quit();
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
    if(!is_run) quit();
}


