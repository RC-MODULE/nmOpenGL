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
    print_thread = nullptr;
    profilerView = new ProfilerView();

    ui->start_button->setEnabled(false);
    ui->stop_button->setEnabled(false);
    ui->ProfilerButton->setEnabled(false);
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
    if (board) delete board;
    qDebug() << "board closed";
    delete profilerView;
    qDebug() << "profilerView closed";
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
    if(board && board->isOpened()){
        QFile file0(ui->program0_filename->text());
        QFile file1(ui->program1_filename->text());
        if(!file0.exists() || !file1.exists()){
            QErrorMessage err(this);
            err.showMessage("Program not selected");
            err.exec();
            printError("Program not selected");
            return;
        }
        if(!program->is_run){
            try{
                board->connectToCore(0);
                board->connectToCore(1);
                board->loadProgram(ui->program0_filename->text().toStdString().c_str(), 0);
                board->loadProgram(ui->program1_filename->text().toStdString().c_str(), 1);
                qDebug() << "board program 0: " << board->programNames[0];
                qDebug() << "board program 1: " << board->programNames[1];
            } catch(std::exception &e){
                printError(e.what());
                return;
            }
            qDebug() << "program loaded";

            program->is_run = true;
            program->start();
            qDebug() << "start";

            //print_thread->is_run = true;
            //print_thread->start();

            ui->profilerCheck->setEnabled(false);
        } else{
            qDebug() << "already started";
        }
    } else {
        printError("Board not opened");
    }

}


void MainWindow::on_stop_button_clicked()
{
    qDebug() << "stoping...";
    ui->profilerCheck->setEnabled(true);
    ui->ProfilerButton->setEnabled(false);
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
        printError(e.what());
    }
}


void MainWindow::on_connect_button_toggled(bool checked)
{
    qDebug() << "connect: " << board ;
    if(checked){
        try {
            board->open();
        } catch (std::exception &e) {
            printError(e.what());
            ui->OpenButton->setChecked(false);
            return;
        }
        ui->start_button->setEnabled(true);
        ui->stop_button->setEnabled(true);
        qDebug() << "Board opened";
    } else{
        try {
            board->close();
        } catch (std::exception &e) {
            printError(e.what());
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
    if(profilerView){
        profilerView->show();
    }
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
            qDebug() << "board opened";
        } catch(std::exception &e){
            printError(e.what());
            ui->OpenButton->setChecked(false);
            if(board) delete board;
            return;
        }

        try{
            board->reset();
            qDebug() << "board reseted";
        }catch(std::exception &e){
            printError(e.what());
        } catch (...){
            printError("Unknown error");
        }

        program = new HostProgram(board);
        program->initedProgramEvent.attach([this](){
            profilerView->tableView->setModel(program->model);
            ui->ProfilerButton->setEnabled(true);
        });
        program->refreshImageEvent.attach([this](){
            QImage image((const uchar *)program->getImage(), 768, 768, QImage::Format_RGB32);
            ui->imagedraw->setPixmap(QPixmap::fromImage(image).scaled(ui->imagedraw->size()));
            ui->imagedraw->update();
            profilerView->tableView->viewport()->update();
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
        delete board;
        board = nullptr;
        qDebug() << "board closed";
        ui->connect_button->setEnabled(false);
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
