#ifndef PCB_H
#define PCB_H

#include"Instruction.h"
#include<QString>
#include<QList>
#include<list>
#include<QDebug>
class PCB;
using PCBList=QList<PCB*>;
class PCB
{
public:
    using InsList=std::list<Instruction>;
    explicit PCB(int id);
    ~PCB();
    void addIns(Instruction ins); 
    /**
     * @brief 更新当前进程状态  
     * 
     * @param time 时间片大小 
     * @return int 返回时间片剩余时间
     *             >0  运行完毕，时间片有剩余
     *             ==0 恰好运行完毕
     *             <0  没有运行完毕
     */
    int tick(int time);
    inline InstructionType curInsType()const
    {
        if(!m_insList.empty())
            return m_insList.front().iType;
        return InstructionType::NONE;
    }
    inline const Instruction curIns()const
    {
        if(!m_insList.empty())
            return m_insList.front();
        return {InstructionType::NONE,0};
    }
    inline size_t insCount()
    {
        return m_insList.size();
    }
    inline int id()const
    {
        return m_id;
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
};



#endif//PCB_H