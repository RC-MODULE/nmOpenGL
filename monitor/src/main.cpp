#include "mainwindow.h"
#include <QApplication>
#include <QDebug>


int main(int argc, char* argv[]){
    QApplication a(argc, argv);
    qDebug() << "monitor thread: " << QThread::currentThread();
    qDebug() << "monitor core applictaion: " << QCoreApplication::instance();
    qDebug() << "argc: " << argc;
    MainWindow w;
    w.show();
    return a.exec();
}
