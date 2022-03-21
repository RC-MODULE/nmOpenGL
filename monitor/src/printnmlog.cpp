#include "printnmlog.h"
#include <QDebug>
#include <QLabel>




PrintNmLog::PrintNmLog(BoardMC12101 *_board) : QObject(nullptr){
    try{
        board = _board;
        filename[0] = "log_nmc0.txt";
        filename[1] = "log_nmc1.txt";
        board->setIO(0, filename[0]);
        board->setIO(1, filename[1]);

        qDebug() << "io services loaded";
    } catch (std::exception e){
        qCritical() << e.what();
    } catch (...){
        qCritical() << "Ups!";
    }

    //timer.setInterval(1000);
    //connect(&timer, &QTimer::timeout, this, &PrintNmLog::update);

}

PrintNmLog::~PrintNmLog(){
    board->closeIO(0);
    board->closeIO(1);
}


void PrintNmLog::start(){
    qDebug() << "PrintNmLogThread start";
    try{
        qDebug() << "log: program name0:" << board->getProgramName(0);
        qDebug() << "log: program name1:" << board->getProgramName(1);
        board->openIO(board->getProgramName(0), 0);
        board->openIO(board->getProgramName(1), 1);

        file[0].setFileName(filename[0]);
        file[1].setFileName(filename[1]);

        if(file[0].open(QFile::ReadOnly | QFile::Text) &&
           file[1].open(QFile::ReadOnly | QFile::Text)){
            textstream[0].setDevice(&file[0]);
            textstream[1].setDevice(&file[1]);
        } else{
            qCritical() << "PrintNmLogThread error";
        }
        emit started();
    } catch (std::exception &e){
        qCritical() << e.what();
    }
}

void PrintNmLog::stop(){
    board->closeIO(0);
    board->closeIO(1);
    file[0].close();
    file[1].close();
    emit finished();
}

void PrintNmLog::update(){

    board->flushIO(0);
    board->flushIO(1);

    while(!textstream[0].atEnd()){
        QString temp = textstream[0].readLine();
        if(!temp.isEmpty()){
            emit updated(temp, 0);
        }
    }

    while(!textstream[1].atEnd()){
        QString temp = textstream[1].readLine();
        if(!temp.isEmpty()){
            emit updated(temp, 1);
        }
    }


}

void PrintNmLog::run(){
    start();
    while(is_run){
        QThread::msleep(500);
        update();
    }
    stop();
}
