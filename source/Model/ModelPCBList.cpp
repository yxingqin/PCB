#include "ModelPCBList.h"
#include <QDebug>
QHash<int, QByteArray> ModelPCBList::roleNamesHash = {
    {PCBID, "pcbId"},
    {INSTYPE, "insType"},
    {TIMELEN, "timeLen"}
    };

ModelPCBList::ModelPCBList(QObject *parent)
    : QAbstractListModel(parent), m_pcbList(nullptr), m_showModel(ShowModel::CURSTATUS)
{
}

int ModelPCBList::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_pcbList->size();
}
QHash<int, QByteArray> ModelPCBList::roleNames() const
{   
    return roleNamesHash;
}

QVariant ModelPCBList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // TODO: model实现
    int row = index.row();
    if (row < m_pcbList->size())
    {
        auto pcb = m_pcbList->at(row);
        auto ins = pcb.curIns();
        switch (role)
        {
        case PCBID:
            return QString::number(pcb.id());
        case INSTYPE:
            return QString(ins.insTypeChar());
        case TIMELEN:
            if (m_showModel == ShowModel::CURSTATUS)
                return QString::number(ins.runTime);
            return QString::number(pcb.totalTime());
        }
    }
    return QVariant();
}

void ModelPCBList::updateData()
{
    beginResetModel();
    endResetModel();
}
