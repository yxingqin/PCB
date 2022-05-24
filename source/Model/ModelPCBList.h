#ifndef MODELPCBLIST_H
#define MODELPCBLIST_H
#include<QHash>
#include <QAbstractListModel>
#include"PCB.h"

class ModelPCBList : public QAbstractListModel
{
    Q_OBJECT
    enum MyRoles
    {
        PCBID=Qt::DisplayRole+1,
        INSTYPE,
        TIMELEN
    };
    static QHash<int,QByteArray> roleNamesHash; 

public:
    enum class ShowModel{//显示模式
        CURSTATUS=0,//当前指令状态： id + 指令 + 该指令的时常
        GLOBSTATUS//全局状态：id+ 周转时间
    };
    explicit ModelPCBList(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QHash<int,QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    inline void setPCBList(PCBList* list)
    {
        m_pcbList=list;   
    }
    inline void setShowModel(ShowModel model)
    {
        m_showModel=model;
    }
    void updateData();
private:
    PCBList* m_pcbList;
    ShowModel m_showModel;
};

#endif // MODELPCBLIST_H
