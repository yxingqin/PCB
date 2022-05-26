#include "PCB.h"
#include <QDateTime>
#include <QThread>
#include "Simulator.h"
PCB::PCB(int id) : m_id(id), m_totalTime(0), m_beinTime(0)
{
}
PCB::~PCB() {}

bool PCB::tick(int ticktime)
{
    auto &ins = m_insList.front();
    bool isOver = false;
    //如果是CPU 指令 使用 slepp 代替 消耗时间
    //如果是其他指令 更新剩余时间
    if (ins.runTime > ticktime) //当前时间片能否运行完毕
    {
        if (ins.iType == InstructionType::CPU)
        {
            Simulator::printLog(QString("p%1 执行CPU指令%2 ms").arg(m_id).arg(ticktime));
            QThread::msleep(ticktime);
        }
        ins.runTime -= ticktime;
    }
    else
    {
        if (ins.iType == InstructionType::CPU)
        {
            Simulator::printLog(QString("p%1 执行CPU指令%2 ms").arg(m_id).arg(ins.runTime));
            QThread::msleep(ins.runTime);
        }
        m_insList.pop_front();
        isOver = true;
    }
    // 指令执行完了 计算周转时间
    if (m_insList.empty())
        m_totalTime += QDateTime::currentMSecsSinceEpoch() - m_beinTime;
    return isOver;
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