#ifndef PROFILERVIEW_H
#define PROFILERVIEW_H
#include <QWidget>
#include <QThread>
#include <QTableView>
#include <QAbstractTableModel>
#include <QVariant>

#include "boardmc12101.h"

class ProfilerModel;

class ProfilerThread : public QThread{
public:
    ProfilerThread(BoardMC12101 *board);
    ~ProfilerThread();
    std::atomic<bool> is_run;
private:
    BoardMC12101 *m_board;
    void run();
};

class ProfilerView : public QWidget
{
    Q_OBJECT
public:
    ProfilerView(BoardMC12101 *board, QWidget *parent = nullptr);
    ~ProfilerView();

private:
    ProfilerThread *profilerThread;
    ProfilerModel *model;
    QTableView *tableView;
};



class ProfilerModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ProfilerModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};
#endif // PROFILERVIEW_H
