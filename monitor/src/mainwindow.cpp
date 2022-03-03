#include "mainwindow.h"
#include <QLabel>
#include <QTime>
#include <QThread>
#include <QDebug>
#include <QTextStream>

#include "mc12101load.h"
#include <iostream>
#include "printnmlog.h"
#include "nmgl_host_init.h"



using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label = ui->imagedraw;

    log[0] = new PrintNmLog(this, ui->io_nmc0);
    log[0]->show();
    log[1] = new PrintNmLog(this, ui->io_nmc1);
    log[1]->show();



    if (BoardMC12101::getBoardCount() < 1){
        qCritical() << "Error: Can't find board";
    }
    try{
        board = new BoardMC12101(0);
        board->connectToCore(0);
        board->connectToCore(1);
    } catch(BoardMC12101Error e){
        qCritical() << e.what() << ": error: " << e.details();
        exit(2);
    }

    refresh = new Refresh(board, label);
    hostInit = new NMGL_HostInit(board, refresh, log);
    hostInit->start();
    label->show();
}

MainWindow::~MainWindow()
{
    refresh->is_run = false;
    hostInit->wait();
    delete hostInit;
    delete log[0];
    delete log[1];
    delete refresh;
    delete board;
    delete ui;
}

