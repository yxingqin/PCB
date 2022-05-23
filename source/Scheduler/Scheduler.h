#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <QObject>
#include "PCB.h"
#include <list>
/**
 * @brief 时间片轮转算法模拟实现
 *
 * 放在一个单独的线程进行模拟
 */
class Simulator;
class Scheduler : public QObject
{
    friend Simulator;
    Q_OBJECT
private:
    int m_timeSliceLen;//时间片长度 
    // 处理机 只有一个 ,需要轮询，
    //I/O Wait队列 设置假设存在无限个
    PCBList m_readyQue;//就绪队列等待使用CPU
    PCBList m_inputQue;//读取等待
    PCBList m_outputQue;//写入等待队列
    PCBList m_waitQue;//其他等待队列
    PCBList m_overQue;//完成队列 
    int m_totaltime;//调度总时长
    bool m_running;
    bool m_stopFlag;
private:
    //将根据当指令类型 将pcb移动到指定队列的队尾
    void moveQue(PCB* pcb,PCBList* from,int index=0);
public:
    explicit Scheduler(QObject* parent=nullptr);
    inline void setTimeSliceLen(int timeSlice)
    {
       m_timeSliceLen=timeSlice;
    }

    inline void setReadyQue(PCBList&& list)
    {
        m_readyQue=std::move(list);
    }

    inline void stop()
    {
        m_stopFlag=false;
    }

    inline int getTotalTime()
    {
        return m_totaltime;
    }
    inline bool isRunning()
    {
        return m_running;
    }
public slots:
    void run();//开始调度 
    void clear();//清空调度状态
signals:
    void updateModel(int model);//更新对于model的信号
};

#endif // SHADULER_H
