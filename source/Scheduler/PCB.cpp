#include "PCB.h"
#include <QDateTime>
PCB::PCB(int id) : m_id(id), m_totalTime(0), m_timePoint(0)
{
}
PCB::~PCB() {}

int PCB::tick(int ticktime)
{

    int tmp = ticktime - m_insList.front().runTime;
    
    if (tmp >= 0)
    {
        m_totalTime += m_insList.front().runTime;
        m_insList.pop_front(); //删除改指令
    }
    else
        m_insList.front().runTime -= ticktime; //更新指令时间
    //计算消耗时间
    m_totalTime += QDateTime::currentMSecsSinceEpoch() - m_timePoint;
    m_timePoint = QDateTime::currentMSecsSinceEpoch(); //记录上一次的时间
    return tmp;
}
void PCB::addIns(Instruction ins)
{
    m_insList.push_back(ins);
}

QString PCB::toString() const
{
    QString ret;
    ret.append(QString("pcbId: %1  ").arg(m_id));
    for (auto &it : m_insList)
    {
        ret.append(it.toString() + ' ');
    }
    return ret;
}