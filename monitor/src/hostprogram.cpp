#include "hostprogram.h"
#include "printnmlog.h"
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <exception>


HostProgram::HostProgram(QObject *parent) : QObject(parent){
    is_run = false;
    mProfilerEnabled = false;

    hostImageIsRefreshing = true;

    model = new ProfilerModel(this);

}

void HostProgram::setBoard(BoardMC12101 *board){
    m_board = board;
    model->setBoard(board);
}

int *HostProgram::getImage(){
    return imageDraw;
}
HostProgram::~HostProgram(){
    delete model;
}

void HostProgram::setProfileEnabled(bool check){
    //nmglConnector.setProfile(check);
    mProfilerEnabled = check;
}

void HostProgram::run(){
    try {
        nmglConnector.setBoard(m_board);
        m_board->connectToCore(0);

        m_board->setTimeout(500);
        int handshake = m_board->sync(0xC0DE0086, 0);
        if(!is_run){
            emit finished();
            return;
        }
        if (handshake != 0xC0DE0000) {
            throw std::runtime_error("Error: Handshake with mc12101-nmc0 wrong!");
        }
        qDebug() << "Handshake passed";
        m_board->setTimeout(0);
        PL_Addr remoteNmAddr = m_board->sync(0xC0DE0086, 0);
        nmglConnector.connect(remoteNmAddr);
        float progress = 0;
        qDebug() << "progress: " << progress;
        while(nmglConnector.initializingProcess() < 1){
            if(progress != nmglConnector.initializingProcess()){
                progress = nmglConnector.initializingProcess();
                qDebug() << "progress: " << progress;
            }
            QThread::msleep(2);
        }
        fb = nmglConnector.framebuffer();
        qDebug() << "framebuffer: " << std::hex << fb;
        qDebug() << "head: " << std::hex << nmglConnector.profilerHeadAddr();
        model->init(nmglConnector.profilerHeadAddr());
    }
    catch (std::exception &e){
        qCritical() << __FILE__ << ":" << __LINE__ << ": error: " << e.what();
        is_run = false;
        emit finished();
        return;
    }
    emit inited();

    while(is_run){
        if(fb == NULL) continue;

        while(frameBufferIsEmpty(fb)){
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        if(profilerEnabled()) model->updateList();
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



