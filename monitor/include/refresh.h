#ifndef REFRESH_H
#define REFRESH_H
#include <QLabel>
#include <QThread>
#include "demo3d_host.h"
#include "framebuffer.h"
#include "mc12101load.h"
#include "boardmc12101.h"


#define WIDTH_IMAGE 768
#define HEIGHT_IMAGE 768

#define COUNT_HOST_IMAGE_BUFFER 8
typedef int Image[WIDTH_IMAGE * HEIGHT_IMAGE];


class Refresh : public QThread{
private:
    NMGL_Framebuffer *fb;
    NMGL_Framebuffer local;
    QLabel* _label;

    BoardMC12101 *board;

    int imageTemp[WIDTH_IMAGE * HEIGHT_IMAGE];
    int imageDraw[WIDTH_IMAGE * HEIGHT_IMAGE];

    void readFramebufferNM(NMGL_Framebuffer &framebuffer, NMGL_Framebuffer *remoteAddr);
    bool frameBufferIsEmpty(NMGL_Framebuffer *remoteAddr);
    void frameBufferIncTail(NMGL_Framebuffer *remoteAddr);
    void readColorBackNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
    void readColorFrontNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
    void readDepthNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);


public:
    Refresh(BoardMC12101 *board, NMGL_Framebuffer* framebuffer, QLabel* label, PL_Access *access);
    ~Refresh();
protected:

    void run();

};

#endif // REFRESH_H
