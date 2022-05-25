#include "Scheduler.h"
#include <QThread>
#include "Simulator.h"
#include <QDateTime>
Scheduler::Scheduler(QObject *parent)
    : QObject(parent), m_running(false), m_stopFlag(false),
      m_timeSliceLen(100), m_totaltime(0)
{
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
       int64_t begin = QDateTime::currentMSecsSinceEpoch();
       for(auto p : m_readyQue)
       {
           p->setBeginTime(begin);
       }
        while (m_stopFlag)
        {
            // TODO: 模拟调度程序
            begin = QDateTime::currentMSecsSinceEpoch();
            /* 处理 就绪队列
            while(true)
                if 就绪队列为空 ：
                    处理其他队列
                if 队头当前指令是CPU指令 do 消耗时间片 (使用 sleep进行模拟) :
                    if 该指令运行完毕（时间片存在剩余 tick()>=0 :
                       根据下一个指令移动对应队列的队尾
                    else :// 该指令没有执行完毕
                        移动到队尾，处理其他队列
                else
                    移动到对应队列
            */
            while (!m_readyQue.isEmpty())
            {
                PCB *pcb = m_readyQue.first();
                if (pcb->curInsType() == InstructionType::CPU)
                {
                    int rtime = pcb->tick(m_timeSliceLen);
                    int runtime = rtime > 0 ? m_timeSliceLen - rtime : m_timeSliceLen;
                    QThread::msleep(runtime);
                    Simulator::printLog(QString("p%1 执行CPU指令%2 ms").arg(pcb->id()).arg(runtime));
                    if (rtime >= 0)
                        moveQue(pcb, m_readyQue, 0);
                    else
                    {
                        auto size = m_readyQue.size();
                        if (size > 1)
                        {
                            m_readyQue.move(0, size - 1); //移动到队尾
                        }
                        break;
                    }
                }
                else
                {
                    //其他指令移动到 对应队列中
                    moveQue(pcb, m_readyQue, 0);
                }
            }
            int ticktime = QDateTime::currentMSecsSinceEpoch() - begin;
            if (ticktime < m_timeSliceLen) //如果 就绪队列为空 没人消耗 cpu  ，这里 休眠cpu 防止卡死
            {
                QThread::msleep(20);
                ticktime += 20;
            }
            /*处理其他队列
                假设 i/o wait 的资源无限，也就说  他们都可以并行运行
                遍历每一个进程, 模拟执行对应的指令（减去对应的处理机占用时间，简单估算为 ticktime）
                if 该指令执行完毕 :
                    根据下一个指令移动到新的队列
            */
            for (auto i = 0; i < m_inputQue.size(); ++i)
            {
                if (m_inputQue[i]->tick(ticktime) >= 0)
                {
                    moveQue(m_inputQue[i], m_inputQue, i);
                    --i;
                }
            }
            for (auto i = 0; i < m_outputQue.size(); ++i)
            {
                if (m_outputQue[i]->tick(ticktime) >= 0)
                {

                    moveQue(m_outputQue[i], m_outputQue, i);
                    --i;
                }
            }
            for (auto i = 0; i < m_waitQue.size(); ++i)
            {
                if (m_waitQue[i]->tick(ticktime) >= 0)
                {

                    moveQue(m_waitQue[i], m_waitQue, i);
                    --i;
                }
            }
            m_totaltime += QDateTime::currentMSecsSinceEpoch() - begin;
            //判断 所有任务队列是否都为空 如果为都为空 那么模拟完毕
            if (m_readyQue.isEmpty() && m_inputQue.isEmpty() && m_outputQue.isEmpty() && m_waitQue.isEmpty())
            {
                Simulator::printInfo(QString("模拟调度完成, 一共用时：%1 ms").arg(m_totaltime));
                for (auto p : m_overQue)
                    Simulator::printLog(QString("p%1 一共用时：%2 ms").arg(p->id()).arg(p->totalTime()));
                m_stopFlag = false;
            }
            emit tick();
        }
        m_running = false;
    }
}

void Scheduler::moveQue(PCB *pcb, PCBList &from, int index)
{
    from.removeAt(index);
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