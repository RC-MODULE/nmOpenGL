#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define WIDTH_IMAGE 768
#define HEIGHT_IMAGE 768

#include <QMainWindow>
#include <QLabel>

#include "hostprogram.h"
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

private slots:
    void on_program0_clicked();

    void on_program1_clicked();

    void on_start_button_clicked();

    void on_stop_button_clicked();

private:
    PrintNmLog *log[2];
    BoardMC12101 *board;

    HostProgram *program;

    NMGL_Framebuffer *framebuffer = 0;
    PrintNmLogThread *print_thread;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
