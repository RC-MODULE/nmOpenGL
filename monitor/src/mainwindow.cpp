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

#define printError(message) qCritical() << __FILE__ << ":" << __LINE__ << ": " << message

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->program0, &QPushButton::clicked, this, [this](){this->setAbsFile(this->ui->program0_filename);});
    connect(ui->program1, &QPushButton::clicked, this, [this](){this->setAbsFile(this->ui->program1_filename);});

    board = nullptr;
    program = nullptr;

    ui->start_button->setEnabled(false);
    ui->stop_button->setEnabled(false);
    ui->loadProgramButton->setEnabled(false);
    on_OpenButton_toggled(true);


    program = new HostProgram(board);
    program->initedProgramEvent.attach([this](){
        ui->profilerTableView->setModel(program->model);
    });
    program->refreshImageEvent.attach([this](){
        QImage image((const uchar *)program->getImage(), 768, 768, QImage::Format_RGB32);
        ui->imagedraw->setPixmap(QPixmap::fromImage(image).scaled(ui->imagedraw->size()));
        ui->imagedraw->update();
    });

    program->refreshImageEvent.attach([this](){
        static int m_frameCount = 0;
        static int m_timeFrameCount = 0;
        static QTime m_time;
        if (m_timeFrameCount == 0) {
             m_time.start();
        } else {
            float fps = float(m_timeFrameCount) / (float(m_time.elapsed()) / 1000);
            ui->statusbar->showMessage(QString("%1 fps. %2 frames").arg(fps).arg(m_frameCount));
        }
        m_frameCount++;
        m_timeFrameCount++;
        if(m_time.elapsed() > 5000){
            m_time.start();
            m_timeFrameCount = 0;
        }
    });

    program->programFinished.attach([this](){
        if(ui->stop_button->isEnabled()){
            ui->stop_button->click();
        }
    });
    qDebug() << "program created";


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
    if (board) delete board;
    qDebug() << "board closed";
    delete ui;
    qDebug() << "delete ui";
}

void MainWindow::setAbsFile(QLineEdit *outFilename){
    QFileDialog dialog(this);
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
    if(!program->is_run){
        program->is_run = true;
        program->start();
        qDebug() << "start";

        ui->profilerCheck->setEnabled(false);
        ui->start_button->setEnabled(false);
        ui->stop_button->setEnabled(true);
    } else{
        qDebug() << "already started";
    }
}

void MainWindow::on_stop_button_clicked()
{
    qDebug() << "stoping...";
    qDebug() << "wait program";
    if(program && program->is_run){
        program->is_run = false;
        program->wait();
    }

    ui->profilerCheck->setEnabled(true);
    ui->start_button->setEnabled(true);
    ui->stop_button->setEnabled(false);
    qDebug() << "stop";
}


void MainWindow::on_connect_button_toggled(bool checked)
{
    if(checked){
        try {
            board->open();
            board->connectToCore(0);
            board->connectToCore(1);
        } catch (std::exception &e) {
            printError(e.what());
            ui->OpenButton->setChecked(false);
            return;
        }
        ui->loadProgramButton->setEnabled(true);
        ui->start_button->setEnabled(true);
        qDebug() << "Board opened";
    } else{
        ui->stop_button->click();
        try {
            board->disconnectFromCore(0);
            board->disconnectFromCore(1);
            board->close();
        } catch (std::exception &e) {
            printError(e.what());
        }
        ui->loadProgramButton->setEnabled(false);
        ui->start_button->setEnabled(false);
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

void MainWindow::on_OpenButton_toggled(bool checked)
{
    qDebug() << "opening board";
    if(checked){
        try{
            int count = BoardMC12101Local::getBoardCount();
            if (count < 1){
                throw std::runtime_error("Error: Can't find board");
            }
            qDebug() << "Founded " << count << " boards";

            board = new BoardMC12101Local(0);

            ui->connect_button->setEnabled(true);
            qDebug() << "board opened";
        } catch(std::exception &e){
            printError(e.what());
            ui->OpenButton->setChecked(false);
            if(board) delete board;
            return;
        }
    } else{

        qDebug() << "Main destructor";
        if(program && program->is_run){
            program->is_run = false;
            program->wait();
        }
        qDebug() << "program quit";
        ui->connect_button->setChecked(false);
        ui->connect_button->setEnabled(false);
        delete board;
        board = nullptr;
        qDebug() << "board closed";
    }
}


void MainWindow::on_getImageCheck_toggled(bool checked)
{
    if(program){
        program->hostImageIsRefreshing = checked;
    } else{
        qWarning() << "Program not exist";
    }
}

void MainWindow::on_resetButton_clicked()
{
    try{
        if(board && board->isOpened())
            board->reset();
        else
            throw std::runtime_error("Board not opened");
        qDebug() << "board reseted";
    }catch(std::exception &e){
        printError(e.what());
    } catch (...){
        printError("Unknown error");
    }
}

void MainWindow::on_loadProgramButton_clicked()
{
    QFile file0(ui->program0_filename->text());
    QFile file1(ui->program1_filename->text());

    if(!file0.exists() || !file1.exists()){
        QErrorMessage err(this);
        err.showMessage("Program not selected");
        err.exec();
        printError("Program not selected");
        return;
    }
    try{
        board->loadProgram(ui->program0_filename->text().toStdString().c_str(), 0);
        board->loadProgram(ui->program1_filename->text().toStdString().c_str(), 1);
        qDebug() << "board program 0: " << board->programNames[0];
        qDebug() << "board program 1: " << board->programNames[1];
    } catch(std::exception &e){
        printError(e.what());
        return;
    }
    qDebug() << "program loaded";
}

