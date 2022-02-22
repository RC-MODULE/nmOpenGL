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

    void boardWork();


private:
    PrintNmLog *log[2];
    BoardMC12101 *board;
    NMGL_Framebuffer *framebuffer = 0;
    Refresh *refresh;
    QLabel *label;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
