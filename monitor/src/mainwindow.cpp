#include "mainwindow.h"
#include <QLabel>
#include <QTime>
#include <QThread>
#include <QDebug>
#include <QTextStream>

#include "mc12101load.h"
#include <iostream>
#include "printnmlog.h"



using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label = ui->imagedraw;

    log[0] = new PrintNmLog(this);
    log[0]->setGeometry(ui->io_nmc0->geometry());
    log[0]->show();
    log[1] = new PrintNmLog(this);
    log[1]->setGeometry(ui->io_nmc1->geometry());
    log[1]->show();


    if (BoardMC12101::getBoardCount() <= 0){
        qCritical() << "Error: Can't find board";
    }
    board = new BoardMC12101(0);
    board->connectToCore(0);
    board->connectToCore(1);

    for(int i = 0; i < MC12101_COUNT_OF_CORES; i++){
        board->setIO(i, &log[i]->out, &log[i]->err, &log[i]->in);
    }

    label->show();
}

void MainWindow::boardWork(){
    try {
        board->loadProgram("main0d.abs", 0);
        board->loadProgram("main1d.abs", 1);
        qDebug() << "program loaded";
        board->openIO("main0d.abs", 0);
        board->openIO("main1d.abs", 1);
        qDebug() << "io services loaded";

        log[0]->start();
        log[1]->start();

        int handshake = board->sync(0xC0DE0086, 0);
        if (handshake != 0xC0DE0000) {
            qCritical() << "Error: Handshake with mc12101-nmc0 wrong!";
            exit(handshake);
        }
        qDebug() << "Handshake passed";

        framebuffer = (NMGL_Framebuffer *)board->sync(0, 0);
        qDebug() << "Framebuffer addr: 0x" << hex << framebuffer << endl;
        //cout << "Image size: " << NMGL_GetFrameWidth() << "x" << NMGL_GetFrameHeight() << endl;
        //Trace("Get image pointer");


        float* vertices = new float[4000 * 12];
        float* normal = new float[4000 * 9];

        const char* filePath = "../models/NM_32.stl";

        PL_Addr verticesNM, normalNM;
        int amount;
        FILE* fmodel;

        fmodel = fopen(filePath, "r");
        if(fmodel == 0){
            qCritical() << "Can't open file";
            exit(2);
        }
        amount = get_amm_poligone(fmodel);
        fclose(fmodel);
        fmodel = fopen(filePath, "r");
        createArrayVec4(fmodel, vertices, normal, 0.5);
        fclose(fmodel);
        board->sync(amount, 0);
        verticesNM = board->sync(0, 0);
        board->writeMemBlock((PL_Word *)vertices, verticesNM, amount * 12);
        normalNM = board->sync(0, 0);
        board->writeMemBlock((PL_Word *)normal, normalNM, amount * 9);

        qDebug() << "First model loaded";

        filePath = "../models/2_sphere.stl";
        fmodel = fopen(filePath, "r");
        if(fmodel == 0){
            qCritical() << "Can't open file";
            exit(2);
        }
        amount = get_amm_poligone(fmodel);
        fclose(fmodel);
        fmodel = fopen(filePath, "r");
        createArrayVec4(fmodel, vertices, normal, 0.5);
        fclose(fmodel);
        board->sync(amount, 0);
        verticesNM = board->sync(0, 0);
        board->writeMemBlock((PL_Word *)vertices, verticesNM, amount * 12);
        normalNM = board->sync(0, 0);
        board->writeMemBlock((PL_Word *)normal, normalNM, amount * 9);


        qDebug() << "Second model loaded";
        board->sync(4);


        delete[] vertices;
        delete[] normal;
    }
    catch (BoardMC12101Error e){
        qCritical() << e.what();
        exit(0);
    }
    catch (...){
        qCritical() << "WTF!!";
        exit(0);
    }


    refresh = new Refresh(board, framebuffer, label, board->getAccess(0));
    refresh->start();
}


MainWindow::~MainWindow()
{
    refresh->terminate();
    log[0]->terminate();
    log[1]->terminate();
    delete refresh;
    delete board;
    delete ui;
}

