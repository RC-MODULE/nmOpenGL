#include "printnmlog.h"
#include <QDebug>
#include <QLabel>

PrintNmLog::PrintNmLog(QWidget *parent, QLabel *label) :
    QPlainTextEdit(parent)
{
    setReadOnly(true);
    setGeometry(label->geometry());
}

PrintNmLog::~PrintNmLog(){
}

void PrintNmLog::appendMessage(const QString& text)
{
    this->appendPlainText(text); // Adds the message to the widget
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom
    //m_logFile.write(text); // Logs to file
}

PrintNmLogThread::PrintNmLogThread(BoardMC12101 *_board, PrintNmLog *_logs[]){

    try{
        board = _board;
        filename[0] = "log_nmc0.txt";
        filename[1] = "log_nmc1.txt";
        board->setIO(0, filename[0]);
        board->setIO(1, filename[1]);
        //board->setIO(0, &std::cout, &std::cerr, &std::cin);
        board->openIO(board->program_name[0], 0);
        board->openIO(board->program_name[1], 1);
        file[0] = new QFile(filename[0]);
        file[1] = new QFile(filename[1]);
        if(file[0]->open(QFile::ReadOnly | QFile::Text) &&
           file[1]->open(QFile::ReadOnly | QFile::Text)){
            textstream[0] = new QTextStream(file[0]);
            textstream[1] = new QTextStream(file[1]);
        } else{
            qCritical() << "PrintNmLogThread error";
        }

        logs[0] = _logs[0];
        logs[1] = _logs[1];
        qDebug() << "io services loaded";
    } catch (std::exception e){
        qCritical() << e.what();
    } catch (...){
        qCritical() << "Ups!";
    }

    is_run = true;

}

PrintNmLogThread::~PrintNmLogThread(){
    board->closeIO(0);
    board->closeIO(1);

    delete textstream[0];
    delete textstream[1];
    delete file[0];
    delete file[1];

}

void PrintNmLogThread::run(){
    while(is_run){
        msleep(1000);
        //board->flushIO(0);
        //board->flushIO(1);

        QString temp;
        *textstream[0] >> temp;
        if (temp != "")
            logs[0]->appendMessage(temp);
        temp.clear();
        *textstream[1] >> temp;
        if (temp != "")
            logs[1]->appendMessage(temp);

    }
}
