#include "mainwindow.h"
#include <QApplication>


using namespace std;


int main(int argc, char* argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    w.boardWork();

    a.exec();

    return 0;
}
