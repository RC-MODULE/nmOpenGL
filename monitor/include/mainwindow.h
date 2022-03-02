#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define WIDTH_IMAGE 768
#define HEIGHT_IMAGE 768

#include <QMainWindow>
#include <QLabel>

#include "refresh.h"
#include "framebuffer.h"
#include "boardmc12101.h"
#include "printnmlog.h"
#include "nmgl_host_init.h"

#include "ui_mainwindow.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadProgram(const char *filename, int core);

private:
    PrintNmLog *log[2];
    BoardMC12101 *board;
    NMGL_HostInit *hostInit;

    Refresh *refresh;
    QLabel *label;

    NMGL_Framebuffer *framebuffer = 0;
    PrintNmLogThread *print_thread;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
