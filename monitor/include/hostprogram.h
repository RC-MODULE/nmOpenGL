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


class HostProgram: public QThread{
private:


    void readFramebufferNM(NMGL_Framebuffer &framebuffer, NMGL_Framebuffer *remoteAddr);
    NMGL_Framebuffer local;
protected:
    int imageTemp[WIDTH_IMAGE * HEIGHT_IMAGE];
    int imageDraw[WIDTH_IMAGE * HEIGHT_IMAGE];

    BoardMC12101 *m_board = 0;
    QLabel* m_label;

    bool frameBufferIsEmpty(NMGL_Framebuffer *remoteAddr);
    void frameBufferIncTail(NMGL_Framebuffer *remoteAddr);
    void readColorBackNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
    void readColorFrontNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
    void readDepthNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);

public:
    std::atomic<bool> is_run;

    NMGL_Framebuffer *fb;
    HostProgram(BoardMC12101 *board, QLabel* imageArea, QLabel *logNm0Area = 0, QLabel *logNm1Area = 0){
        m_board = board;
        m_label = imageArea;
        is_run = true;
    }
    ~HostProgram();

protected:
    void init();

    virtual void host_main() = 0;
private:
    void run(){
        init();
        host_main();
    }
};

#endif // REFRESH_H
