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

class ProfilerModel : public QAbstractTableModel
{
private:
    Q_OBJECT

public:
    ProfilerModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void setBoard(BoardMC12101 *board);


    BoardMC12101 *mBoard;
    int head;

    void init();

    std::map<unsigned, QString> funcNames;
    std::vector<ProfilerData> profilerVector;

    void updateList();
};
#endif // PROFILERVIEW_H
