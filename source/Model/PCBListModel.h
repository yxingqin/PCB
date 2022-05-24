#ifndef PCBListModel_H
#define PCBListModel_H
#include<QHash>
#include <QAbstractListModel>
#include"PCB.h"

class PCBListModel : public QAbstractListModel
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

    explicit PCBListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QHash<int,QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //操作数据的接口
    void clear();
    void setPCBList(PCBList&& list);
    inline bool isEmpty()const
    {
        return m_pcbList.isEmpty();
    }
    inline size_t size()const
    {
        return m_pcbList.size();
    }
    inline PCB* at(int i)
    {
        return m_pcbList.at(i);   
    }
    PCB* first()
    {
        if(m_pcbList.isEmpty())
            return nullptr;
        return m_pcbList.first();
    }
    void pop();//删除第一行数据
    void push(PCB* pcb);//插入到末尾
    void updateRow(int row);//更新第row行数据
    void rotate();//轮转将第一个元素移动到末尾
    void removeAt(int i);
    void updateAll();//更新所有数据
    inline void setShowModel(ShowModel model)
    {
        m_showModel=model;
    }
    inline const PCBList& pcbList()const
    {
        return m_pcbList;
    }
private:
    PCBList m_pcbList;
    ShowModel m_showModel;
};

#endif // PCBListModel_H
