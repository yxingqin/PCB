#include"Scheduler.h"
#include<QThread>
Scheduler::Scheduler(QObject* parent)
:QObject(parent),m_running(false),m_stopFlag(false),
m_timeSliceLen(0),m_totaltime(0)
{   
}

void Scheduler::clear()
{
    stop();
    QThread::msleep(10);
    m_readyQue.clear();
    m_inputQue.clear();
    m_outputQue.clear();
    m_waitQue.clear();
}

void Scheduler::run()
{
    if(!m_running)
    {
        m_running=true;

        m_stopFlag=true;
        while(m_stopFlag)
        {
            //TODO: 模拟调度程序

        }        
        m_running=false;   
    }
}