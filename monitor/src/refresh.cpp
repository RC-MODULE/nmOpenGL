#include "refresh.h"
#include <iostream>
#include <QImage>
#include <QTime>
#include <QDebug>
#include "mc12101load.h"
#include "demo3d_host.h"

using namespace std;


Refresh::Refresh(BoardMC12101 *_board, QLabel* label)
{
    board = _board;
    _label = label;
}

Refresh::~Refresh()
{

}

void Refresh::run(){
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

    while(true){
        if(fb == NULL) continue;

        while(frameBufferIsEmpty(fb)){
            msleep(2);
        }
        readColorFrontNM(imageTemp, fb, 0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);
        frameBufferIncTail(fb);
        for(int y = 0; y < HEIGHT_IMAGE; y++){
            for(int x = 0; x < WIDTH_IMAGE; x++){
                imageDraw[y * WIDTH_IMAGE + x] = imageTemp[(HEIGHT_IMAGE - y - 1) * WIDTH_IMAGE + x];
            }
        }
        QImage image((const uchar *)imageDraw, 768, 768, QImage::Format_RGB32);
        _label->setPixmap(QPixmap::fromImage(image).scaled(_label->size()));
        _label->update();
    }
}
