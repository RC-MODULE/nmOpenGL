#include "hostprogram.h"
#include "printnmlog.h"
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <exception>

HostProgram::HostProgram(BoardMC12101 *board, QObject *parent) : QObject(parent){
    m_board = board;
    is_run = false;
    profilerEnabled = false;

    hostImageIsRefreshing = true;


    model = new ProfilerModel(m_board, parent);

}

int *HostProgram::getImage(){
    return imageDraw;
}
HostProgram::~HostProgram(){
    delete model;
}


void HostProgram::run(){
    if(!init()){
        emit finished();
        return;
    }

    while(is_run){
        if(fb == NULL) continue;

        while(frameBufferIsEmpty(fb)){
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        if(profilerEnabled) model->updateList();
        if(hostImageIsRefreshing){
            readColorFrontNM(imageTemp, fb, 0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);
            for(int y = 0; y < HEIGHT_IMAGE; y++){
                for(int x = 0; x < WIDTH_IMAGE; x++){
                    imageDraw[y * WIDTH_IMAGE + x] = imageTemp[(HEIGHT_IMAGE - y - 1) * WIDTH_IMAGE + x];
                }
            }
        }
        frameBufferIncTail(fb);
        emit update();
    }
    emit finished();
}

bool HostProgram::init(){
    try {
        m_board->setTimeout(500);
        int handshake = m_board->sync(0xC0DE0086, 0);
        m_board->setTimeout(0);
        if(!is_run) return false;
        if (handshake != 0xC0DE0000) {
            throw std::runtime_error("Error: Handshake with mc12101-nmc0 wrong!");
        }
        qDebug() << "Handshake passed";


        fb = (NMGL_Framebuffer *)m_board->sync(profilerEnabled, 0);
        if(!is_run) return false;
        qDebug() << "Framebuffer addr: " << hex << fb;

        if(profilerEnabled){
            model->head = m_board->sync(0, 0);
            model->init();
            if(!is_run) return false;
            qDebug() << "Get profiler head addr: 0x" << hex << model->head;
        }
    }
    catch (std::exception &e){
        qCritical() << __FILE__ << ":" << __LINE__ << ": error: " << e.what();
        is_run = false;
        return false;
    }
    if(!is_run) return false;
    emit inited();
    return true;
}


