#ifndef REFRESH_H
#define REFRESH_H
#include <QLabel>
#include <QThread>
#include "framebuffer.h"
#include "mc12101load.h"
#include "boardmc12101.h"


#define WIDTH_IMAGE 768
#define HEIGHT_IMAGE 768

#define COUNT_HOST_IMAGE_BUFFER 8
typedef int Image[WIDTH_IMAGE * HEIGHT_IMAGE];


class Refresh{
private:
    NMGL_Framebuffer local;
    QLabel* _label;

    BoardMC12101 *board = 0;

    int imageTemp[WIDTH_IMAGE * HEIGHT_IMAGE];
    int imageDraw[WIDTH_IMAGE * HEIGHT_IMAGE];

    void readFramebufferNM(NMGL_Framebuffer &framebuffer, NMGL_Framebuffer *remoteAddr);
    bool frameBufferIsEmpty(NMGL_Framebuffer *remoteAddr);
    void frameBufferIncTail(NMGL_Framebuffer *remoteAddr);
    void readColorBackNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
    void readColorFrontNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
    void readDepthNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);

public:
    std::atomic<bool> is_run;

    NMGL_Framebuffer *fb;
    Refresh(BoardMC12101 *board, QLabel* label);
    ~Refresh();

    void run();

};

#endif // REFRESH_H
