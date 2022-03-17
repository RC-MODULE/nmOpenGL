#ifndef PROFILERVIEW_H
#define PROFILERVIEW_H
#include <QWidget>
#include <QThread>
#include <QTableView>
#include <QAbstractTableModel>
#include <QVariant>
#include <map>
#include <vector>
#include <string>

#include "boardmc12101.h"
#include "nmprofiler.h"

using namespace std;

class ProfilerModel;


class ProfilerView : public QWidget
{
    Q_OBJECT
public:
    ProfilerView(QWidget *parent = nullptr);
    ~ProfilerView();

    QTableView *tableView;

};


class ProfilerModel : public QAbstractTableModel
{
private:
    Q_OBJECT

public:
    ProfilerModel(BoardMC12101 *board, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;


    bool mapFileExisted;
    BoardMC12101 *mBoard;
    char mMapFile[1024];


    std::map<unsigned, QString> funcNames;
    std::vector<ProfilerData> profilerVector;

    void updateList();
};
#endif // PROFILERVIEW_H
