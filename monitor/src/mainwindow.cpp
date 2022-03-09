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

//using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->program0_filename->setReadOnly(true);
    ui->program1_filename->setReadOnly(true);

    connect(ui->program0, &QPushButton::clicked, this, [this](){this->setAbsFile(this->ui->program0_filename);});
    connect(ui->program1, &QPushButton::clicked, this, [this](){this->setAbsFile(this->ui->program1_filename);});

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

    print_thread = new PrintNmLogThread(board, ui->log_nm0, ui->log_nm1);
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
    profiler->close();
    delete profiler;
    qDebug() << "delete profiler";
    delete board;
    qDebug() << "board closed";
    delete ui;
    qDebug() << "delete ui";
}

void MainWindow::setAbsFile(QLineEdit *outFilename){
    QFileDialog dialog(this);
    dialog.setDirectory("..");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Programs (*.abs)"));
    QStringList name;
    if(dialog.exec()){
        name = dialog.selectedFiles();
        if(!name.isEmpty())
            outFilename->setText(name[0]);
    }
}


void MainWindow::on_start_button_clicked()
{
    if(board->isOpened()){
        if(ui->program0_filename->text().isEmpty() || ui->program1_filename->text().isEmpty()){
            QErrorMessage err(this);
            err.showMessage("Program not selected");
            err.exec();
            qCritical() << "Program not selected";
            return;
        }
        qDebug() << "program 0: " << ui->program0_filename->text();
        qDebug() << "program 1: " << ui->program1_filename->text();
        try{
            board->loadProgram(ui->program0_filename->text().toStdString().c_str(), 0);
            board->loadProgram(ui->program1_filename->text().toStdString().c_str(), 1);
            qDebug() << "board program 0: " << board->programNames[0];
            qDebug() << "board program 1: " << board->programNames[1];
        } catch(std::exception &e){
            qCritical() << e.what();
        }
        qDebug() << "program loaded";

        program->is_run = true;
        print_thread->is_run = true;

        print_thread->start();
        program->start();
        //profiler = new ProfilerView(board);
        //ui->profilerCheck->setEnabled(false);
        qDebug() << "start";
    } else {
        qCritical() << "Board not opened";
    }

}


void MainWindow::on_stop_button_clicked()
{
    qDebug() << "stoping...";
    //delete profiler;
    //ui->profilerCheck->setEnabled(true);
    qDebug() << "wait program";
    if(program && program->is_run){
        program->is_run = false;
        program->wait();
    }
    qDebug() << "stop";
    if(print_thread && print_thread->is_run){
        print_thread->is_run = false;
        print_thread->wait();
    }
    qDebug() << "print stop";
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

void MainWindow::on_profilerCheck_stateChanged(int arg1)
{
    program->profilerEnabled = arg1;
    if(program->profilerEnabled )
        qDebug() << "Profiler enabled";
    else
        qDebug() << "Profiler disabled";
}

void MainWindow::on_ProfilerButton_clicked()
{
    profiler->show();
}
