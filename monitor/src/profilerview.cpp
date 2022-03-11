#include "profilerview.h"
//#include "hostprofiler.h"
#include <QDebug>


ProfilerView::ProfilerView(BoardMC12101 *board, QWidget *parent) : QWidget(parent){
    resize(600, 800);
    tableView = new QTableView(this);
    tableView->resize(size());
    model = new ProfilerModel();
    tableView->setModel(model);
    profilerThread = new ProfilerThread(board);
    profilerThread->start();
}
ProfilerView::~ProfilerView(){
    if(profilerThread && profilerThread->is_run){
        profilerThread->is_run = false;
        profilerThread->wait();
    }
    delete profilerThread;
    delete model;
    delete tableView;
}

ProfilerThread::ProfilerThread(BoardMC12101 *board){
    is_run = false;
    m_board = board;
}

ProfilerThread::~ProfilerThread(){
    is_run = true;
}

void ProfilerThread::run(){
    while(is_run){
        msleep(2000);
        //halProfilerPrint2xml();
        qDebug() << "Profiler model";
    }
}


ProfilerModel::ProfilerModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    qDebug() << "Profiler model";
}

int ProfilerModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
}

int ProfilerModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 5;
}

QVariant ProfilerModel::data(const QModelIndex &index, int role) const {
    return QVariant();
}
