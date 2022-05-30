#include "Scheduler.h"
#include <QThread>
#include "Simulator.h"
#include <QDateTime>
Scheduler::Scheduler(QObject *parent)
    : QObject(parent), m_running(false), m_stopFlag(false),
      m_timeSliceLen(100), m_totaltime(0), m_curPcbId(0)
{
}
Scheduler::~Scheduler()
{
    clear();
}
void Scheduler::clear()
{
    stop();
    QThread::msleep(10);
    for (int i = 0; i < m_readyQue.size(); ++i)
        delete m_readyQue[i];
    m_readyQue.clear();
    for (auto p : m_inputQue)
        delete p;
    m_inputQue.clear();
    for (auto p : m_outputQue)
        delete p;
    m_outputQue.clear();
    for (auto p : m_waitQue)
        delete p;
    m_waitQue.clear();
    for (auto p : m_overQue)
        delete p;
    m_overQue.clear();
    m_totaltime = 0;
}

void Scheduler::run()
{
    if (!m_running)
    {
        Simulator::printInfo("开始模拟执行命令");
        m_running = true;
        m_stopFlag = true;
        int ticktime = 0;
        //统计开始运行时间
        int64_t begin = QDateTime::currentMSecsSinceEpoch();
        for (auto p : m_readyQue)
        {
            p->setBeginTime(begin);
        }
        //调度循环
        while (m_stopFlag)
        {
            m_curPcbId = 0;                                       //表示当前没有进程占用cpu
            int64_t tpoint = QDateTime::currentMSecsSinceEpoch(); //用于记录处理机消耗时间
            //处理就绪队列
            if (!m_readyQue.isEmpty())
            {
                PCB *pcb = m_readyQue.first();
                m_readyQue.pop_front(); //移除就绪队列 进入运行态
                m_curPcbId = pcb->id();
                if (pcb->curInsType() == InstructionType::CPU)
                    if (pcb->tick(m_timeSliceLen))
                        movePCBToQue(pcb); //执行完了 移动到 对应队列 否则周转
                    else
                        m_readyQue.push_back(pcb);
                else
                    //其他指令移动到  移动到指定队列中
                    movePCBToQue(pcb); //这里消耗很少 所以直接进入下一个时间片 也就是不break了
            }
            // scheduler重新拿到CPU 去检查其他队列，之后在进入下一轮时间片
            ticktime = QDateTime::currentMSecsSinceEpoch() - tpoint; //记录消耗时间
            if (ticktime <= 5)                                       //休眠cpu 防止界面更新太快导致 卡死
            {
                QThread::msleep(5);
                ticktime += 5;
            }
            //处理其他队列
            for (auto i = 0; i < m_inputQue.size(); ++i)
            {
                if (m_inputQue[i]->tick(ticktime))
                {
                    pushReadQue(m_inputQue[i], m_inputQue, i);
                    --i;
                }
            }
            ticktime = QDateTime::currentMSecsSinceEpoch() - tpoint; //记录消耗时间
            for (auto i = 0; i < m_outputQue.size(); ++i)
            {
                if (m_outputQue[i]->tick(ticktime))
                {
                    pushReadQue(m_outputQue[i], m_outputQue, i);
                    --i;
                }
            }
            ticktime = QDateTime::currentMSecsSinceEpoch() - tpoint; //记录消耗时间
            for (auto i = 0; i < m_waitQue.size(); ++i)
            {
                if (m_waitQue[i]->tick(ticktime))
                {
                    pushReadQue(m_waitQue[i], m_waitQue, i);
                    --i;
                }
            }
            //判断 所有任务队列是否都为空 如果为都为空 那么模拟完毕
            if (m_readyQue.isEmpty() && m_inputQue.isEmpty() && m_outputQue.isEmpty() && m_waitQue.isEmpty())
            {
                m_stopFlag = false;
                emit over();
                m_curPcbId = 0; //表示当前没有进程占用cpu
            }
            emit tick(); //更新界面
        }
        for (auto p : m_overQue)
            Simulator::printLog(QString("p%1 一共用时：%2 ms").arg(p->id()).arg(p->totalTime()));
        // 计算调度用时
        m_totaltime = QDateTime::currentMSecsSinceEpoch() - begin;
        Simulator::printInfo(QString("模拟调度完成, 调度用时：%1 ms").arg(m_totaltime));
        m_running = false;
    }
}

void Scheduler::movePCB(PCB *pcb, PCBList &from, int index)
{
    from.removeAt(index);
    movePCBToQue(pcb);
}
void Scheduler::movePCBToQue(PCB *pcb)
{
    switch (pcb->curInsType())
    {
    case InstructionType::CPU:
    {
        Simulator::printLog(QString("p%1 加入 readyQue").arg(pcb->id()));
        m_readyQue.push_back(pcb);
        break;
    }
    case InstructionType::INPUT:
    {
        Simulator::printLog(QString("p%1 加入InputQue").arg(pcb->id()));
        m_inputQue.push_back(pcb);
        break;
    }
    case InstructionType::OUTPUT:
    {
        Simulator::printLog(QString("p%1 加入OutputQue").arg(pcb->id()));
        m_outputQue.push_back(pcb);
        break;
    }
    case InstructionType::WAIT:
    {
        Simulator::printLog(QString("p%1 加入WaitQue").arg(pcb->id()));
        m_waitQue.push_back(pcb);
        break;
    }
    default:
    {
        Simulator::printLog(QString("p%1 运行完毕").arg(pcb->id()));
        m_overQue.push_back(pcb);
        break;
    }
    }
}
void Scheduler::pushReadQue(PCB *pcb, PCBList &list, int index)
{
    Simulator::printLog(QString("p%1 加入ReadQue").arg(pcb->id()));
    list.removeAt(index);
    m_readyQue.push_back(pcb);
}