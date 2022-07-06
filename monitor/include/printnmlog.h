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
public:
    stringstream out[2];
    stringstream err[2];
    stringstream in[2];

    const char *filename[2];

    QTextStream textstream[2];
    QFile file[2];

    BoardMC12101 *board;

    PrintNmLog();
    void setBoard(BoardMC12101 *board);
    ~PrintNmLog();

    std::atomic_bool is_run;

signals:
    void started();
    void updated(QString text, int core);
    void finished();
public slots:
    void run();
private slots:
    void start();
    void update();
    void stop();

};

#endif // REFRESH_H
