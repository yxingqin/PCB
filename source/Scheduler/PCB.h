#ifndef PCB_H
#define PCB_H

#include"Instruction.h"
#include<list>
#include<QString>
class PCB
{
public:
    using InsList=std::list<Instruction>;
    PCB(int id);
    void addIns(Instruction&& ins); 
    /**
     * @brief 更新当前进程状态  
     * 
     * @param time 时间片大小 
     * @return int 返回0表示时间片已经用尽，返回正数表示当前指令运行完毕之后剩余时间片，返回-1 表示进程恰好运行完毕
     */
    int tick(int time);
    ~PCB();

    inline int id()const
    {
        return m_id;
    }
    inline  InsList& insList()
    {
        return m_insList;
    }
    inline size_t insCount()
    {
        return m_insList.size();
    }
    QString toString();
private:
    int m_id;//进程id
    InsList m_insList;//指令表
    int m_totalTime;//进程周转的时间
    int m_curIns;//当前运行到的指令索引
};



#endif//PCB_H