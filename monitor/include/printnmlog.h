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
#include "boardmc12101.h"

using namespace std;

class PrintNmLogThread : public QThread
{
public:
    stringstream out[2];
    stringstream err[2];
    stringstream in[2];

    const char *filename[2];
    QPlainTextEdit *logs[2];

    QTextStream *textstream[2];
    QFile *file[2];

    BoardMC12101 *board;
    std::atomic<bool> is_run;

    PrintNmLogThread(BoardMC12101 *board, QPlainTextEdit *plane0, QPlainTextEdit *plane1);
    ~PrintNmLogThread();

    void run();
};

#endif // REFRESH_H
