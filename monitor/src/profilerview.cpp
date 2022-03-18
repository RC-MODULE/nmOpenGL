#include "profilerview.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
//#include "hostprofiler.h"
#include "nmprofiler.h"

#include "boardmc12101.h"

#include <algorithm>


#include <fstream>

unsigned map_symbol2address(const char* mapfile, const char* symbol);
bool map_address2symbol(char* mapfile, unsigned addr, char* fullname);
bool gccmap_address2symbol(char* mapfile, unsigned addr, char* fullname);
unsigned profiler_head_address(char* mapfile);
char* profiler_funcname2char(unsigned* funcname );
int  profiler_readfunc(BoardMC12101 *board, int addr, ProfilerData* prof, int processor);
int  profiler_count(BoardMC12101 *board, unsigned head_addr, int processor);
int profiler_read(BoardMC12101 *board, unsigned head_addr, ProfilerData* profile, int count, int processor);

#define printError(message) qCritical() << __FILE__ << ":" << __LINE__ << ": " << message

void ProfilerModel::init(){
    int count = profiler_count(mBoard, head, 0);
    qDebug() << hex << "count: " << count;
    profilerVector.resize(count);
}

void ProfilerModel::updateList(){
    int count = profiler_count(mBoard, head, 0);

    ProfilerData *data = new ProfilerData[count];
    profiler_read(mBoard, head, data, count, 0);

    //qDebug() << "row count before: " <<

    for(int i = 0; i < count; i++){
        QString funcName((char *)data[i].funcname);
        funcNames[data[i].funcaddr] = funcName;
        profilerVector[i] = data[i];
    }
    std::sort(profilerVector.begin(), profilerVector.end(), [](const ProfilerData &a, const ProfilerData &b){return a.summary > b.summary; });
    delete[] data;    
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex( count, 6);
    emit dataChanged( topLeft, bottomRight );
}

ProfilerModel::ProfilerModel(BoardMC12101 *board, QObject *parent)
    : QAbstractTableModel(parent)
{
    mBoard = board;
    head = 0;
    //updateList();
}

int ProfilerModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);

    return profilerVector.size();
}

int ProfilerModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 6;
}

QVariant ProfilerModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        QString columnNames[] = {"Function", "Summary", "Calls", "Average", "PerElement", "Address"};
        return columnNames[section];
    }
    return QVariant();
}

QVariant ProfilerModel::data(const QModelIndex &index, int role) const {

    if(role == Qt::DisplayRole){
        switch(index.column()){
        case 0:
            return funcNames.at(profilerVector[index.row()].funcaddr);
        case 1:
            return profilerVector[index.row()].summary;
        case 2:
            return profilerVector[index.row()].calls;
        case 3:
            return (float)profilerVector[index.row()].summary / profilerVector[index.row()].calls;
        case 4:
            return (float)profilerVector[index.row()].summary / profilerVector[index.row()].calls / profilerVector[index.row()].size_summary;
        case 5:
            return QString("0x%1").arg(profilerVector[index.row()].funcaddr, 8, 16, QLatin1Char('0'));

        }
    }
    return QVariant();
}


char* profiler_funcname2char(unsigned* funcname )	{
    char* str86=(char*)funcname;
    int i;
    for(i=0;i<MAX_FUNCNAME_LENGTH; i++){
        str86[i]=funcname[i];
        if (str86[i]==0)
            break;
    }
    str86[i] = 0;
    return str86;
}

int  profiler_readfunc(BoardMC12101 *board, int addr, ProfilerData* prof, int processor){
    try{
        board->readMemBlock((PL_Addr)addr, prof, sizeof(ProfilerData)/4, processor);
    } catch(std::exception &e){
        printError(e.what());
        return 0;
    }
    profiler_funcname2char(prof->funcname);
    return prof->funcaddr;
}


int  profiler_count(BoardMC12101 *board, unsigned head_addr, int processor) {
    int count = 0;
    int next_field=0x777;
    while (next_field) {
        try{
            board->readMemBlock(head_addr, &next_field, 1, processor);
        } catch (std::exception &e){
            printError(e.what());
            return count;
        }
        head_addr += next_field;
        count++;
    }
    return count;
}


int profiler_read(BoardMC12101 *board, unsigned head_addr, ProfilerData* profile, int count, int processor){

    for (int i = 0; i < count; i++) {
        profiler_readfunc(board, head_addr, profile + i, processor);
        head_addr += profile->next;
    }
    return 0;
}
