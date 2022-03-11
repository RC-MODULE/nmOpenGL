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

    board = nullptr;
    program = nullptr;
    print_thread = nullptr;

    ui->start_button->setEnabled(false);
    ui->stop_button->setEnabled(false);
    on_OpenButton_toggled(true);
}

MainWindow::~MainWindow()
{
    qDebug() << "Main destructor";
    if(program && program->is_run){
        program->is_run = false;
        program->wait();
    }
    qDebug() << "program quit";
    if (program) delete program;
    qDebug() << "delete program";
    /*if(profiler != nullptr) {
        profiler->close();
        delete profiler;
    }
    qDebug() << "delete profiler";*/
    if (board) delete board;
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
    if(board && board->isOpened()){
        if(ui->program0_filename->text().isEmpty() || ui->program1_filename->text().isEmpty()){
            QErrorMessage err(this);
            err.showMessage("Program not selected");
            err.exec();
            qCritical() << "Program not selected";
            return;
        }
        if(!program->is_run){
            qDebug() << "program 0: " << ui->program0_filename->text();
            qDebug() << "program 1: " << ui->program1_filename->text();
            try{
                board->loadProgram(ui->program0_filename->text().toStdString().c_str(), 0);
                board->loadProgram(ui->program1_filename->text().toStdString().c_str(), 1);
                qDebug() << "board program 0: " << board->programNames[0];
                qDebug() << "board program 1: " << board->programNames[1];
            } catch(std::exception &e){
                qCritical() << e.what();
                return;
            }
            qDebug() << "program loaded";

            program->is_run = true;
            //print_thread->is_run = true;

            //print_thread->start();
            program->start();
            qDebug() << "start";
        } else{
            qDebug() << "already started";
        }
        //profiler = new ProfilerView(board);
        //ui->profilerCheck->setEnabled(false);
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
    /*if(print_thread && print_thread->is_run){
        print_thread->is_run = false;
        print_thread->wait();
    }
    qDebug() << "print stop";*/
    try{
        if(board) board->reset();
    } catch(BoardMC12101Error &e){
        qWarning() << e.what();
    }
}


void MainWindow::on_connect_button_toggled(bool checked)
{
    qDebug() << "connect: " << board ;
    if(checked){
        try {
            board->open();
            board->connectToCore(0);
            board->connectToCore(1);
        } catch (std::exception &e) {
            qCritical() << e.what();
            ui->OpenButton->setChecked(false);
            return;
        }
        ui->start_button->setEnabled(true);
        ui->stop_button->setEnabled(true);
        qDebug() << "Board opened";
    } else{
        try {
            board->disconnectFromCore(0);
            board->disconnectFromCore(1);
            board->close();
        } catch (std::exception &e) {
            qCritical() << e.what();
        }
        ui->start_button->setEnabled(false);
        ui->stop_button->setEnabled(false);
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

void MainWindow::on_OpenButton_toggled(bool checked)
{
    qDebug() << "opening board";
    if(checked){
        try{
            int count = BoardMC12101Local::getBoardCount();
            if (count < 1){
                throw std::exception("Error: Can't find board");
            }
            qDebug() << "Founded " << count << " boards";

            board = new BoardMC12101Local(0);
            qDebug() << "board opened";
        } catch(std::exception &e){
            qCritical() << e.what();
            ui->OpenButton->setChecked(false);
            if(board) delete board;
            return;
        }

        try{
            board->reset();
            qDebug() << "board reseted";
        }catch(std::exception e){
            qCritical() << e.what();
        } catch (...){
            qCritical() << "Unknown error";
        }

        program = new HostProgram(board, ui->imagedraw);
        qDebug() << "program created";

        //print_thread = new PrintNmLogThread(board, ui->log_nm0, ui->log_nm1);

        ui->connect_button->setEnabled(true);
    } else{
        qDebug() << "Main destructor";
        if(program && program->is_run){
            program->is_run = false;
            program->wait();
        }
        qDebug() << "program quit";
        delete program;
        program = nullptr;
        qDebug() << "delete program";
        /*if(profiler != nullptr) {
            profiler->close();
            delete profiler;
        }
        qDebug() << "delete profiler";*/
        delete board;
        board = nullptr;
        qDebug() << "board closed";
        ui->connect_button->setEnabled(false);
    }
}


