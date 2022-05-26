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
     * @param ticktime   当执行cpu 指令时，表示给与时间片大小 ，当执行其他指令时  表示 处理机消耗时间
     * @return bool 返回当前指令是否执行完毕
     */
    bool tick(int ticktime);
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
    inline void setBeginTime(int64_t tpoint)
    {
        m_beinTime=tpoint;
    }
    QString toString()const;
private:
    int m_id;//进程id
    InsList m_insList;//指令表
    int64_t m_beinTime;//开始运行时间
    int m_totalTime;//进程周转的时间
};



#endif//PCB_H