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

#include "profilerview.h"

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

    void setAbsFile(QLineEdit *outFilename);

private slots:

    void on_start_button_clicked();

    void on_stop_button_clicked();

    void on_connect_button_toggled(bool checked);

    void on_profilerCheck_stateChanged(int arg1);

    void on_ProfilerButton_clicked();

    void on_OpenButton_toggled(bool checked);

    void on_getImageCheck_toggled(bool checked);

private:


    QPlainTextEdit *log[2];
    BoardMC12101 *board;

    HostProgram *program;


    ProfilerView *profilerView;

    NMGL_Framebuffer *framebuffer = 0;
    PrintNmLogThread *print_thread;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
