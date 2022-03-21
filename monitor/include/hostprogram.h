#ifndef REFRESH_H
#define REFRESH_H
#include <QLabel>
#include <QDebug>
#include <QThread>
#include "framebuffer.h"
#include "mc12101load.h"
#include "boardmc12101.h"
#include "profilerview.h"
#include "iobserver.h"

#include "printnmlog.h"

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
    QString programNames[2];
    std::atomic<bool> is_run;
    bool profilerEnabled;


    ProfilerModel *model;
    QTableView *mTableView;

    bool hostImageIsRefreshing;

    Subject refreshImageEvent;
    Subject initedProgramEvent;
    Subject programFinished;
    PrintNmLogThread *logThread;

    bool init();
    void setLogPlainText(const QPlainTextEdit *log0, const QPlainTextEdit *log1);

    NMGL_Framebuffer *fb;
    HostProgram(BoardMC12101 *board){
        m_board = board;
        is_run = false;
        profilerEnabled = false;

        hostImageIsRefreshing = true;


        model = new ProfilerModel(m_board);
    }

    int *getImage(){
        return imageDraw;
    }
    ~HostProgram(){
        delete model;
    }

protected:
    virtual void run();

};

#endif // REFRESH_H
