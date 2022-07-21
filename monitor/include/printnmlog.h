#ifndef PRINTNMLOG_H
#define PRINTNMLOG_H
#include <QThread>
#include <QLabel>
#include <QPlainTextEdit>
#include <iostream>
#include <QTextStream>
#include <sstream>
#include <fstream>
#include <string>
#include <QScrollBar>
#include <QFile>
#include <QTimer>
#include "boardmc12101.h"

using namespace std;

class PrintNmLog : public QObject
{
    Q_OBJECT
private:
    char programName[2][1024];
    const char *filename[2];
    QTimer timer;
public:
    stringstream out[2];
    stringstream err[2];
    stringstream in[2];


    QTextStream textstream[2];
    QFile file[2];

    BoardMC12101 *board;

    PrintNmLog();
    void setBoard(BoardMC12101 *board);
    ~PrintNmLog();


    bool isRun();
    void setProgram(QString programName, int core);
signals:
    void started();
    void updated(QString text, int core);
    void finished();
public slots:
    void start();
    void stop();
private slots:
    void update();

};

#endif // REFRESH_H
