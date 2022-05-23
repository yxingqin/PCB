#ifndef PCB_H
#define PCB_H

#include"Instruction.h"
#include<QString>
#include<QList>
#include<QDebug>
class PCB;
using PCBList=QList<PCB>;
class PCB
{
public:
    using InsList=QList<Instruction>;
    PCB(int id);
    void addIns(Instruction ins); 
    /**
     * @brief 更新当前进程状态  
     * 
     * @param time 时间片大小 
     * @return int 返回0表示时间片已经用尽，返回正数表示当前指令运行完毕之后剩余时间片，返回-1 表示进程恰好运行完毕
     */
    int tick(int time);
    ~PCB();
    inline const Instruction curIns()const
    {
        //mmp 遇到了迭代器失效的问题 气死了
        //Instruction ins=*m_curInsIt;
        //qDebug()<<"begein"<<m_insList.front().toString();
        //qDebug()<<"curIns"<<m_curInsIt->toString();
        if(m_curInsIt!=m_insList.end())
            return *m_curInsIt;
        return {};
    }
    inline bool isRunOver()const
    {
        return m_curInsIt==m_insList.cend();
    }
    inline int id()const
    {
        return m_id;
    }
    inline const InsList& insList()const
    {
        return m_insList;
    }
    inline size_t insCount()const
    {
        return m_insList.size();
    }
    inline int totalTime()const
    {
        return m_totalTime;
    }
    QString toString();
private:
    int m_id;//进程id
    InsList m_insList;//指令表
    int m_totalTime;//进程周转的时间
    InsList::iterator m_curInsIt;// 指示当前运行到的指令
};



#endif//PCB_H