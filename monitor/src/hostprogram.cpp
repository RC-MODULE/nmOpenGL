#include "hostprogram.h"
#include "printnmlog.h"
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <exception>


void HostProgram::run(){
    if(!init()){
        return;
    }

    while(is_run){
        if(fb == NULL) continue;

        while(frameBufferIsEmpty(fb)){
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        model->updateList();
        if(hostImageIsRefreshing){
            readColorFrontNM(imageTemp, fb, 0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);
            for(int y = 0; y < HEIGHT_IMAGE; y++){
                for(int x = 0; x < WIDTH_IMAGE; x++){
                    imageDraw[y * WIDTH_IMAGE + x] = imageTemp[(HEIGHT_IMAGE - y - 1) * WIDTH_IMAGE + x];
                }
            }
        }
        frameBufferIncTail(fb);
        refreshImageEvent.notify();
    }
    delete model;
}

bool HostProgram::init(){
    try {
        int handshake = m_board->sync(0xC0DE0086, 0);
        if (handshake != 0xC0DE0000) {
            throw std::runtime_error("Error: Handshake with mc12101-nmc0 wrong!");
        }
        qDebug() << "Handshake passed";

        if(!is_run) quit();
        fb = (NMGL_Framebuffer *)m_board->sync(profilerEnabled, 0);
        qDebug() << "Framebuffer addr: " << hex << fb;

        if(profilerEnabled){
            model->head = m_board->sync(0, 0);
            qDebug() << "Get profiler head addr: 0x" << hex << model->head;
        }
        model->init();
    }
    catch (std::exception &e){
        qCritical() << __FILE__ << ":" << __LINE__ << ": error: " << e.what();
        is_run = false;
        quit();
    }
    if(!is_run) quit();
    initedProgramEvent.notify();
    return true;
}


