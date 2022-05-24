#include "PCBListModel.h"
#include <QDebug>
QHash<int, QByteArray> PCBListModel::roleNamesHash = {
    {PCBID, "pcbId"},
    {INSTYPE, "insType"},
    {TIMELEN, "timeLen"}
    };

PCBListModel::PCBListModel(QObject *parent)
    : QAbstractListModel(parent), m_showModel(ShowModel::CURSTATUS)
{
}

int PCBListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_pcbList.size();
}
QHash<int, QByteArray> PCBListModel::roleNames() const
{   
    return roleNamesHash;
}

QVariant PCBListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // TODO: model实现
    int row = index.row();
    if (row < m_pcbList.size())
    {
        auto pcb = m_pcbList.at(row);
        auto ins = pcb->curIns();
        switch (role)
        {
        case PCBID:
            return QString::number(pcb->id());
        case INSTYPE:
            return QString(ins.insTypeChar());
        case TIMELEN:
            if (m_showModel == ShowModel::CURSTATUS)
                return QString::number(ins.runTime);
            return QString::number(pcb->totalTime());
        }
    }
    return QVariant();
}
void  PCBListModel::clear()
{
    if(m_pcbList.isEmpty())
        return;
    beginRemoveRows(QModelIndex(),0,m_pcbList.size()-1);
    for(auto p:m_pcbList)
        delete p;
    m_pcbList.clear();
    endRemoveRows();
}
void PCBListModel::setPCBList(PCBList&& list)
{
    clear();
    beginInsertRows(QModelIndex(),0,list.size()-1);
    m_pcbList =std::move(list);
    endInsertRows();
}
void PCBListModel::pop()
{
    if(m_pcbList.isEmpty())
        return;
    beginRemoveRows(QModelIndex(),0,0);
    m_pcbList.removeFirst();
    endRemoveRows();
}
void PCBListModel::push(PCB* pcb)
{
    beginInsertRows(QModelIndex(),m_pcbList.size(),m_pcbList.size());
    m_pcbList.push_back(pcb);
    endInsertRows();
}
void PCBListModel::updateRow(int row)
{
    if(row<m_pcbList.size())
    {
        auto i= index(row,0,QModelIndex());
        emit dataChanged(i,i,QVector<int>()<<PCBID<<INSTYPE<<TIMELEN);
    }
}
void PCBListModel::rotate()
{
    if(m_pcbList.size()>1)
    {
        beginMoveRows(QModelIndex(),0,0,QModelIndex(),m_pcbList.size());
        m_pcbList.move(0,m_pcbList.size()-1);
        endMoveRows();
    }
}
void  PCBListModel::removeAt(int i)
{
    if(i<m_pcbList.size())
    {
        beginRemoveRows(QModelIndex(),i,i);
        m_pcbList.removeAt(i);
        endRemoveRows();
    }
}
void PCBListModel::updateAll()
{
    if(!m_pcbList.isEmpty())
    {
        auto first = index(0,0,QModelIndex());
        auto last = index(m_pcbList.size()-1,0,QModelIndex());
        emit dataChanged(first,last,QVector<int>()<<PCBID<<INSTYPE<<TIMELEN);
    }
}