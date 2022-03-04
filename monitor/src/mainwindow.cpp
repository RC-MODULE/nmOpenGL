#include "mainwindow.h"
#include <QLabel>
#include <QTime>
#include <QThread>
#include <QDebug>
#include <QTextStream>
#include <QFileDialog>
#include <QErrorMessage>

#include "mc12101load.h"
#include <iostream>
#include "printnmlog.h"

#include "unistd.h"

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->program0_filename->setReadOnly(true);
    ui->program1_filename->setReadOnly(true);

    if (BoardMC12101Local::getBoardCount() < 1){
        qCritical() << "Error: Can't find board";
    }
    try{
        board = new BoardMC12101Local(0);
    } catch(BoardMC12101Error &e){
        qCritical() << e.what();
    }

    try{
        board->reset();
    }catch(std::exception &e){
        qCritical() << e.what();
    }
    program = new HostProgram(board, ui->imagedraw);
}

MainWindow::~MainWindow()
{
    qDebug() << "Main destructor";
    if(program && program->is_run){
        program->is_run = false;
        program->wait();
    }
    qDebug() << "program quit";
    delete program;
    qDebug() << "delete program";
    delete board;
    qDebug() << "board closed";
    delete ui;
    qDebug() << "delete ui";
}


void MainWindow::on_program0_clicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory("..");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Programs (*.abs)"));
    QStringList name;
    if(dialog.exec()){
        name = dialog.selectedFiles();
        if(!name.isEmpty())
            ui->program0_filename->setText(name[0]);
    }
}


void MainWindow::on_program1_clicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory("..");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Programs (*.abs)"));
    QStringList name;
    if(dialog.exec()){
        name = dialog.selectedFiles();
        if(!name.isEmpty())
            ui->program1_filename->setText(name[0]);
    }
}


void MainWindow::on_start_button_clicked()
{
    if(board->isOpened()){
        program->setProgramNamePointer(ui->program0_filename->text(), ui->program1_filename->text());
        program->is_run = true;
        if(ui->program0_filename->text().isEmpty() || ui->program1_filename->text().isEmpty()){
            QErrorMessage err(this);
            err.showMessage("Program not selected");
            err.exec();
        } else{
            program->start();
            qDebug() << "start";
        }
    } else {
        qCritical() << "Board not opened";
    }

}


void MainWindow::on_stop_button_clicked()
{
    qDebug() << "stoping...";
    if(program && program->is_run){
        program->is_run = false;
        program->wait();
    }
    qDebug() << "stop";
    try{
        if(board) board->reset();
    } catch(BoardMC12101Error &e){
        qWarning() << e.what();
    }
}


void MainWindow::on_connect_button_toggled(bool checked)
{
    if(checked){
        try {
            board->open();
            board->connectToCore(0);
            board->connectToCore(1);
        } catch (std::exception &e) {
            qCritical() << e.what();
            //ui->connect_button->setChecked(true);
            ui->connect_button->setChecked(false);
            return;
        }
        qDebug() << "Board opened";
    } else{
        try {
            board->disconnectFromCore(0);
            board->disconnectFromCore(1);
            board->close();
        } catch (std::exception &e) {
            qCritical() << e.what();
            return;
        }
        qDebug() << "Board closed";
    }
}
