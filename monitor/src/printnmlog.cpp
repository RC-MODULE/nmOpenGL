#include "printnmlog.h"
#include <QDebug>

PrintNmLog::PrintNmLog(QWidget *parent) :
    QPlainTextEdit(parent)
{
    setReadOnly(true);
}

PrintNmLog::~PrintNmLog(){

}

void PrintNmLog::appendMessage(const QString& text)
{
    this->appendPlainText(text); // Adds the message to the widget
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom
    //m_logFile.write(text); // Logs to file
}

void PrintNmLog::run(){
    while(true){
        while(out.str().empty()){
            out.flush();
            msleep(100);
        }
        qDebug().nospace() << out.str().c_str();
        QString strs(out.str().c_str());
        appendMessage(strs);

    }
}
