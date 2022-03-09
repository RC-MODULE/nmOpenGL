#include "printnmlog.h"
#include <QDebug>
#include <QLabel>


PrintNmLogThread::PrintNmLogThread(BoardMC12101 *_board, QPlainTextEdit *plane0, QPlainTextEdit *plane1){

    try{
        board = _board;
        filename[0] = "log_nmc0.txt";
        filename[1] = "log_nmc1.txt";
        //board->setIO(0, filename[0]);
        //board->setIO(1, filename[1]);
        //board->setIO(0, &std::cout, &std::cerr, &std::cin);
        //board->setIO(1, &std::cout, &std::cerr, &std::cin);

        logs[0] = plane0;
        logs[1] = plane1;
        qDebug() << "io services loaded";
    } catch (std::exception e){
        qCritical() << e.what();
    } catch (...){
        qCritical() << "Ups!";
    }

    is_run = false;

}

PrintNmLogThread::~PrintNmLogThread(){
    //board->closeIO(0);
    //board->closeIO(1);
}

void PrintNmLogThread::run(){
    qDebug() << "PrintNmLogThread start";
    try{
        qDebug() << "log: program name0:" << board->programNames[0];
        qDebug() << "log: program name1:" << board->programNames[1];
        board->openIO(board->programNames[0], 0);
        board->openIO(board->programNames[1], 1);

        /*file[0] = new QFile(filename[0]);
        file[1] = new QFile(filename[1]);
        if(file[0]->open(QFile::ReadOnly | QFile::Text) &&
           file[1]->open(QFile::ReadOnly | QFile::Text)){
            textstream[0] = new QTextStream(file[0]);
            textstream[1] = new QTextStream(file[1]);
        } else{
            qCritical() << "PrintNmLogThread error";
        }*/
    } catch (std::exception &e){
        qCritical() << e.what();
    } catch (...){
        qCritical() << "Ups2!";
    }

    while(is_run){
        msleep(1000);
        board->flushIO(0);
        board->flushIO(1);
        /*board->closeIO(0);
        QThread::msleep(2000);
        board->openIO(board->programNames[0], 0);*/

        /*QString temp;
        *textstream[0] >> temp;
        if (!temp.isEmpty())
            logs[0]->appendPlainText(temp); // Adds the message to the widget
            logs[0]->verticalScrollBar()->setValue(logs[0]->verticalScrollBar()->maximum()); // Scrolls to the bottom
        temp.clear();
        *textstream[1] >> temp;
        if (temp.isEmpty())
            logs[1]->appendPlainText(temp); // Adds the message to the widget
            logs[1]->verticalScrollBar()->setValue(logs[1]->verticalScrollBar()->maximum()); // Scrolls to the bottom
*/
    }

    board->closeIO(0);
    board->closeIO(1);
    delete textstream[0];
    delete textstream[1];
    delete file[0];
    delete file[1];
}
