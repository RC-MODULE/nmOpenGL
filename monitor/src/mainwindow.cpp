#include "mainwindow.h"
#include <QLabel>
#include <QTime>
#include <QThread>
#include <QDebug>
#include <QTextStream>

#include "mc12101load.h"
#include <iostream>
#include "printnmlog.h"
#include "demo3d_program.h"



using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    program = new Demo3dProgram(board, ui->imagedraw);
    program->start();
    ui->imagedraw->show();
}

MainWindow::~MainWindow()
{
    program->is_run = false;
    program->quit();
    delete log[0];
    delete log[1];
    delete program;
    delete board;
    delete ui;
}

