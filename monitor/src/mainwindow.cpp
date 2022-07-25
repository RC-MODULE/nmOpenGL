#include "mainwindow.h"
#include <QLabel>
#include <QTime>
#include <QThread>
#include <QDebug>
#include <QTextStream>
#include <QFileDialog>
#include <QErrorMessage>
#include <QMessageBox>

#include <iostream>
#include "printnmlog.h"


#include "boardmc12101_local.h"
#include "boardmc12101_remote.h"


//using namespace std;

#define printError(message) qCritical() << __FILE__ << ":" << __LINE__ << ": " << message
void MainWindow::printMessage(const QString &message){
    ui->statusbar->showMessage(message);
    qDebug() << message;
}

void MainWindow::errorMessage(const QString &message){
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
    printMessage(message);
    qCritical() << message;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , hostThread(this->thread())
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->program0, &QPushButton::clicked, this, [this](){this->setAbsFile(this->ui->program0_filename);});
    connect(ui->program1, &QPushButton::clicked, this, [this](){this->setAbsFile(this->ui->program1_filename);});

    connect(ui->ioButton, &QPushButton::clicked, this, [this](){
        nmLog->setBoard(board);
        QFile files[2];
        files[0].setFileName(ui->program0_filename->text());
        files[1].setFileName(ui->program1_filename->text());
        if(files[0].exists() && files[1].exists()){
            nmLog->setProgram(ui->program0_filename->text(), 0);
            nmLog->setProgram(ui->program1_filename->text(), 1);
            if(nmLog->isRun()){
                nmLog->stop();
            }
            nmLog->start();
        }
    });

    connect(ui->localRadioButton, &QRadioButton::toggled, this, [this](bool checked){
        if(checked){
            board = new BoardMC12101Local(0);
            program->setBoard(board);
            nmLog->setBoard(board);
            qDebug() << "create local";
        } else{
            if(board) delete board;
            qDebug() << "delete local";
        }
    });
    connect(ui->remoteRadioButton, &QRadioButton::toggled, this, [this](bool checked){
        ui->remoteAddrLine->setEnabled(checked);
        ui->serverConnectButton->setEnabled(checked);
        if(checked){
            board = new BoardMC12101Remote(0);
            program->setBoard(board);
            nmLog->setBoard(board);
            qDebug() << "create remote";
        } else{
            if(ui->serverConnectButton->isChecked()) ui->serverConnectButton->click();
            if(board) delete board;
            qDebug() << "delete remote";
        }
    });
    connect(ui->serverConnectButton, &QPushButton::toggled, this, [this](bool checked){
        if(checked){
            QUrl url = QUrl(QString("tcp://%1").arg(ui->remoteAddrLine->text()));
            try{
                static_cast<BoardMC12101Remote*>(board)->connectToHost(url.host().toStdString().c_str(), url.port());
                printMessage("Connected to remote server");
            } catch (std::exception &e){
                errorMessage(e.what());
                ui->serverConnectButton->setChecked(false);
                printMessage("Disconnected from remote server");
            }
        } else{
            if(static_cast<BoardMC12101Remote*>(board)->isConnected()){
                static_cast<BoardMC12101Remote*>(board)->disconnectFromHost();
            }
        }
    });


    board = new BoardMC12101Local(0);
    program = new HostProgram();
    nmLog = new PrintNmLog();

    //connect(ui->profilerCheck, &QCheckBox::toggled, program, &HostProgram::setProfileEnabled);

    connect(program, &HostProgram::inited, this, [this](){
        if(program->profilerEnabled()){
            ui->profilerTableView->setModel(program->model);
        }
    });

    connect(program, &HostProgram::update, this, [this](){
        if(program->profilerEnabled()){
            ui->profilerTableView->reset();
        }
    });
    connect(program, &HostProgram::update, this, [this](){
        QImage image((const uchar *)program->getImage(), 768, 768, QImage::Format_RGB32);
        ui->imagedraw->setPixmap(QPixmap::fromImage(image).scaled(ui->imagedraw->size()));
        ui->imagedraw->update();
    });

    connect(program, &HostProgram::update, this, [this](){
        static int m_frameCount = 0;
        static int m_timeFrameCount = 0;
        static QTime m_time;
        if (m_timeFrameCount == 0) {
             m_time.start();
        } else {
            float fps = float(m_timeFrameCount) / (float(m_time.elapsed()) / 1000);
            printMessage(QString("%1 fps. %2 frames").arg(fps).arg(m_frameCount));
        }
        m_frameCount++;
        m_timeFrameCount++;
        if(m_time.elapsed() > 5000){
            m_time.start();
            m_timeFrameCount = 0;
        }
    });

    program->moveToThread(&hostThread);
    connect(&hostThread, &QThread::started, program, &HostProgram::run);

    connect(nmLog, &PrintNmLog::started, this, [this](){
        ui->log_nm0->clear();
        ui->log_nm1->clear();
    });

    connect(nmLog, &PrintNmLog::updated, this, [this](QString text, int core){
        if(core == 0){
            ui->log_nm0->appendPlainText(text); // Adds the message to the widget
            ui->log_nm0->verticalScrollBar()->setValue(ui->log_nm0->verticalScrollBar()->maximum()); // Scrolls to the bottom
        } else{
            ui->log_nm1->appendPlainText(text); // Adds the message to the widget
            ui->log_nm1->verticalScrollBar()->setValue(ui->log_nm1->verticalScrollBar()->maximum()); // Scrolls to the bottom
        }
    });

}

MainWindow::~MainWindow()
{
    qDebug() << "Main destructor";
    hostThread.terminate();
    program->is_run = false;
    qDebug() << "program quit";
    nmLog->stop();
    qDebug() << "nm io quit";
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
    if(!board->isOpened()){
        errorMessage("Board not opened");
        return;
    }
    if(!program->is_run){
        program->is_run = true;
        hostThread.start();

        printMessage("start");

        ui->profilerCheck->setEnabled(false);
    } else{
        errorMessage("already started");
    }
}

void MainWindow::on_stop_button_clicked()
{
    printMessage("stoping...");
    if(nmLog->isRun())
        nmLog->stop();
    hostThread.terminate();
    program->is_run = false;
    ui->profilerCheck->setEnabled(true);
    printMessage("stop");
}


void MainWindow::on_profilerCheck_stateChanged(int arg1)
{
    program->setProfileEnabled(arg1);
}

void MainWindow::on_OpenButton_toggled(bool checked)
{
    qDebug() << "opening board";

    if(checked){
        for(int i = 0; i < 1; i++){
            try{
                board->open();

                program->setBoard(board);
                nmLog->setBoard(board);

                ui->loadProgramButton->setEnabled(true);
                ui->ioButton->setEnabled(true);
                printMessage("Board opened");
                break;
            } catch(std::exception &e){
                errorMessage(e.what());
                ui->OpenButton->setChecked(false);
            }
        }
    } else{
        ui->stop_button->click();
        try {
            board->close();
        } catch (std::exception &e) {
            errorMessage(e.what());
        }
        ui->loadProgramButton->setEnabled(false);
        ui->ioButton->setEnabled(false);
        printMessage("Board closed");
    }
}


void MainWindow::on_getImageCheck_toggled(bool checked)
{
    if(program){
        program->hostImageIsRefreshing = checked;
    } else{
        printMessage("Program not exist");
    }
}

void MainWindow::on_resetButton_clicked()
{
    try{
        if(board->isOpened())
            board->reset();
        else
            throw std::runtime_error("Board not opened");
        printMessage("board reseted");
    }catch(std::exception &e){
        errorMessage(e.what());
    }
}

void MainWindow::on_loadProgramButton_clicked()
{


    QFile file0(ui->program0_filename->text());
    QFile file1(ui->program1_filename->text());

    if(!file0.exists() || !file1.exists()){
        errorMessage("Program not selected");
        return;
    }
    try{
        board->connectToCore(0);
        board->connectToCore(1);
        board->loadProgram(ui->program0_filename->text().toStdString().c_str(), 0);
        board->loadProgram(ui->program1_filename->text().toStdString().c_str(), 1);
        printMessage(QString("board program 0: %1").arg(board->getProgramName(0)));
        printMessage(QString("board program 1: %1").arg(board->getProgramName(1)));
        board->disconnectFromCore(0);
        board->disconnectFromCore(1);
    } catch(std::exception &e){
        errorMessage(e.what());
        return;
    }
    printMessage("program loaded");
}

