#include "mainwindow.h"
#include <QApplication>
#include <QDebug>


int main(int argc, char* argv[]){
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("nmOpenGL monitor");
    MainWindow w;
    w.setWindowTitle(QCoreApplication::applicationName());
    w.show();
    return a.exec();
}
