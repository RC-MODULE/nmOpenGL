#include "hostprogram.h"
#include <iostream>
#include <QImage>
#include <QTime>
#include <QDebug>
#include "mc12101load.h"
#include "demo3d_host.h"
#include <thread>
#include "demo3d_program.h"

using namespace std;

Demo3dProgram::Demo3dProgram(BoardMC12101 *board, QLabel* label):
    HostProgram(board, label){

}

void Demo3dProgram::host_main(){

    const char* filePath = "../models/NM_32.stl";

    PL_Addr verticesNM, normalNM;
    int amount;
    FILE* fmodel;

    //qDebug() << "board: " << board;
    //qDebug() << "access: " << board->getAccess(0) << ", " << board->getAccess(1);
    //qDebug() << "program names: " << board->program_name[0] << ", " << board->program_name[1];
    float *vertices, *normal;
    try{

        vertices = new float[4000 * 12];
        normal = new float[4000 * 9];

        qDebug() << "Load first model:";
        fmodel = fopen(filePath, "r");
        if(fmodel == NULL){
            qCritical() << "Can't open file";
            exit(2);
        }
        amount = get_amm_poligone(fmodel);
        qDebug() << "amount of polygons: " << amount;
        if (fseek(fmodel, 0, SEEK_SET)){
            qCritical() << "errno: " << errno;
            exit(2);
        }
        createArrayVec4(fmodel, vertices, normal, 0.5);
        fclose(fmodel);

        m_board->sync(amount, 0);
        verticesNM = m_board->sync(0, 0);
        qDebug() << "vertices addr on NM: " << hex << verticesNM;
        m_board->writeMemBlock((PL_Word *)vertices, verticesNM, amount * 12);
        normalNM = m_board->sync(0, 0);
        qDebug() << "normal addr on NM: " << hex << normalNM;
        m_board->writeMemBlock((PL_Word *)normal, normalNM, amount * 9);

        qDebug() << "First model loaded";




        qDebug() << "Load second model:";
        filePath = "../models/2_sphere.stl";
        fmodel = fopen(filePath, "r");
        if(fmodel == 0){
            qCritical() << "Can't open file";
            exit(2);
        }
        amount = get_amm_poligone(fmodel);
        qDebug() << "amount of polygons: " << amount;
        if (fseek(fmodel, 0, SEEK_SET)){
            qCritical() << "errno: " << errno;
        }
        createArrayVec4(fmodel, vertices, normal, 0.5);
        fclose(fmodel);

        m_board->sync(amount, 0);
        verticesNM = m_board->sync(0, 0);
        qDebug() << "vertices addr on NM: 0x" << hex << verticesNM;
        m_board->writeMemBlock((PL_Word *)vertices, verticesNM, amount * 12);
        normalNM = m_board->sync(0, 0);
        qDebug() << "normal addr on NM: 0x" << hex << normalNM;
        m_board->writeMemBlock((PL_Word *)normal, normalNM, amount * 9);


        qDebug() << "Second model loaded";
        m_board->sync(4);

    }catch(BoardMC12101Error e){
        qCritical() << e.what() << ": error: " << e.details();
        delete[] vertices;
        delete[] normal;
    } catch(...){
        qCritical() << "Undefined error";
        delete[] vertices;
        delete[] normal;
    }


    while(is_run){
        if(fb == NULL) continue;

        while(frameBufferIsEmpty(fb)){
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        readColorFrontNM(imageTemp, fb, 0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);
        frameBufferIncTail(fb);
        for(int y = 0; y < HEIGHT_IMAGE; y++){
            for(int x = 0; x < WIDTH_IMAGE; x++){
                imageDraw[y * WIDTH_IMAGE + x] = imageTemp[(HEIGHT_IMAGE - y - 1) * WIDTH_IMAGE + x];
            }
        }
        QImage image((const uchar *)imageDraw, 768, 768, QImage::Format_RGB32);
        m_label->setPixmap(QPixmap::fromImage(image).scaled(m_label->size()));
        m_label->update();
    }
}
