#ifndef PRINTNMLOG_H
#define PRINTNMLOG_H
#include <QThread>
#include <QPlainTextEdit>
#include <iostream>
#include <QTextStream>
#include <sstream>
#include <string>
#include <QScrollBar>

using namespace std;

class PrintNmLog : public QPlainTextEdit, public QThread
{
    Q_OBJECT

public:
    stringstream out;
    stringstream err;
    stringstream in;


/* snip */
public:
    PrintNmLog(QWidget *parent);
    ~PrintNmLog();

    void appendMessage(const QString& text);

private:
    void run();

};

#endif // REFRESH_H
