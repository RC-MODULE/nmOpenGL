#include "refresh.h"
#include <iostream>
#include <QImage>
#include <QTime>
#include "mc12101load.h"

using namespace std;


Refresh::Refresh(BoardMC12101 *_board, NMGL_Framebuffer* framebuffer, QLabel* label, PL_Access *access)
{
    board = _board;
    fb = framebuffer;
    _label = label;
    readFramebufferNM(local, fb);
    //cout << "Image size: " << local.width << "x" << local.height << endl;
    //cout << "Access: " << access << endl;
}

Refresh::~Refresh()
{

}

void Refresh::run(){
    while(true){
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
