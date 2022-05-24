#include "Simulator.h"
#include<QThread>
#include"Scheduler.h"
#include"PCB.h"
#include <QQmlApplicationEngine>
QObject *Simulator::window = nullptr;

Simulator::Simulator(QQmlApplicationEngine& engine)
    : QObject(nullptr)
{
    m_scheduler=new Scheduler;
    m_thread=new QThread(this);
    m_scheduler->moveToThread(m_thread);
    m_thread->start();
    connect(m_scheduler,&Scheduler::over,this,&Simulator::over);
    connect(m_scheduler,&Scheduler::tick,this,&Simulator::tick);
    connect(this,&Simulator::doSheduler,m_scheduler,&Scheduler::run);
    //注册属性
    auto context=engine.rootContext();
    context->setContextProperty("$Simulator",this);
    context->setContextProperty("$ReadyQueModel",&m_scheduler->m_readyQue);
    context->setContextProperty("$inputQueModel",&m_scheduler->m_inputQue);
    context->setContextProperty("$ouputQueModel",&m_scheduler->m_outputQue);
    context->setContextProperty("$WaitQueModel",&m_scheduler->m_waitQue);
    context->setContextProperty("$OverModel",&m_scheduler->m_overQue);
    window=engine.rootObjects().first();
}
Simulator::~Simulator()
{
    m_scheduler->stop();
    m_thread->quit();
    m_thread->wait();
}


void Simulator::printLog(const QString &msg)
{
    QMetaObject::invokeMethod(window, "_printMessageLog", Q_ARG(QVariant, msg));
}
void Simulator::printInfo(const QString& msg,const QString& color)
{
    QMetaObject::invokeMethod(window, "_printMessageInfo", Q_ARG(QVariant, msg),Q_ARG(QVariant,color));
}




bool Simulator::loadInsFile(const QString& path)
{
    QFile  file(path);
    if(file.open(QIODevice::Text|QIODevice::ReadOnly))
    {
        QString data =file.readAll();
        file.close();
        //解析文件
        data=data.toUpper();  
        int i=0;
        int len=data.length();
        int procId=0;
        PCBList pcbList;
        while(i<len)//解析进程队列
        { 
            ++procId;         
            while(i<len&&data[i]!='P')
                ++i;
            PCB* pcb=new PCB(procId);
            while(++i<len&&data[i]!='H')//解析进程指令
            {
                auto type=Instruction::CharToType(data[i].toLatin1());
                if(type!=InstructionType::NONE)
                {
                    //获取运行时常
                    int timeLen=0;
                    while(++i<len&&data[i].isDigit())
                    {
                        timeLen=timeLen*10+data[i].digitValue();
                    }
                    if(timeLen>0)
                    {
                        pcb->addIns({type,timeLen});
                    }
                } 
            }
            if(pcb->insCount()>0)
                pcbList.push_back(pcb);   
        }
        if(pcbList.size()>0)
        {
            printInfo("指令加载成功");
            for(auto i:pcbList)
                printLog(i->toString());
            m_scheduler->setReadyQue(std::move(pcbList));
            return true;
        }    
    }
    return false;
}

void Simulator::scheduler(int timeLen)
{
    if(m_scheduler->isRunning())
        return;
    m_scheduler->setTimeSliceLen(timeLen);
    emit doSheduler();
}
