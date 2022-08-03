#include "printnmlog.h"
#include <QDebug>
#include <QLabel>




PrintNmLog::PrintNmLog() : QObject(nullptr){
    connect(&timer, &QTimer::timeout, this, &PrintNmLog::update);
    filename[0] = "log_nmc0.txt";
    filename[1] = "log_nmc1.txt";

    file[0].setFileName(filename[0]);
    file[1].setFileName(filename[1]);

}

void PrintNmLog::setBoard(BoardMC12101 *_board){
    try{
        board = _board;
        board->setIO(0, filename[0]);
        board->setIO(1, filename[1]);
    } catch (std::exception e){
        qCritical() << e.what();
    }
}

void PrintNmLog::setProgram(QString _programName, int core){
    memcpy(programName[core], _programName.toStdString().c_str(), _programName.size() + 1);
}

PrintNmLog::~PrintNmLog(){
    board->closeIO(0);
    board->closeIO(1);
}


void PrintNmLog::start(){
    try{
        if(isRun()){
            qDebug() << "logs already run";
            return;
        }
        board->openIO(programName[0], 0);
        board->openIO(programName[1], 1);

        if(file[0].open(QFile::ReadOnly | QFile::Text) &&
           file[1].open(QFile::ReadOnly | QFile::Text)){
            textstream[0].setDevice(&file[0]);
            textstream[1].setDevice(&file[1]);
        } else{
            qCritical() << "PrintNmLog error";
        }
        timer.start(1000);
        emit started();
    } catch (std::exception &e){
        qCritical() << e.what();
    }
}

bool PrintNmLog::isRun(){
    return timer.isActive();
}

void PrintNmLog::stop(){
    if(timer.isActive()){
        board->closeIO(0);
        board->closeIO(1);
        file[0].close();
        file[1].close();
        timer.stop();
    }
    emit finished();
}

void PrintNmLog::update(){
    board->flushIO(0);
    board->flushIO(1);

    file[0].flush();
    file[1].flush();

    while(!textstream[0].atEnd()){
        int bytes = file[0].bytesAvailable();
        if(bytes > 0){
            auto mes = textstream[0].read(bytes);
            emit updated(mes, 0);
        }
    }

    while(!textstream[1].atEnd()){
        int bytes = file[1].bytesAvailable();
        if(bytes > 0){
            auto mes = textstream[1].read(bytes);
            emit updated(mes, 1);
        }
    }

    /*while(!textstream[1].atEnd()){
        QString temp = textstream[1].readLine();
        if(!temp.isEmpty()){
            emit updated(temp, 1);
        }
    }*/


}

