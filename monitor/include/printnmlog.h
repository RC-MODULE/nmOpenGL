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

class PrintNmLog : public QPlainTextEdit
{
    Q_OBJECT

public:



/* snip */
public:
    PrintNmLog(QWidget *parent, QLabel *label);
    ~PrintNmLog();

    void appendMessage(const QString& text);

};

class PrintNmLogThread : public QThread
{
public:
    stringstream out[2];
    stringstream err[2];
    stringstream in[2];

    const char *filename[2];
    PrintNmLog *logs[2];

    QTextStream *textstream[2];
    QFile *file[2];

    BoardMC12101 *board;

    PrintNmLogThread(BoardMC12101 *board, PrintNmLog *logs[]);
    ~PrintNmLogThread();

    void run();
};

#endif // REFRESH_H
