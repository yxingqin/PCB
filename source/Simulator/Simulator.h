#ifndef SIMULATOR
#define SIMULATOR
#include<QObject>
#include<QtQml>
#include"Scheduler.h"
#include"ModelPCBList.h"
class QQmlApplicationEngine;
class QThread;
/**
 * @brief 进程调度模拟
 * 该类注册到qml中，向qml端提供接口
 * 如 打开文件，加载指令，保存日志文件，开始调度等接口
 */
class Simulator:public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString curRunProc READ curRunProc NOTIFY tick)
private:
    static QObject* window;
    QObject* m_console;
    QThread* m_thread;//模拟线程
    Scheduler* m_scheduler;//调度器
    //每一个队列的视图
    ModelPCBList m_modelReadyQue;
    ModelPCBList m_modelInputQue;
    ModelPCBList m_modelOutputQue;
    ModelPCBList m_modelOverQue;
    ModelPCBList m_modelWaitQue;
public:
    Simulator(QQmlApplicationEngine& engine);
    static void printLog(const QString& msg);//打印日志
    static void printInfo(const QString& info,const QString& color="gold");
    void init(QObject* window);
    virtual ~Simulator();
    /**
     * @brief 载入指令 
     * 
     * @param path 
     * @return bool 返回 是否载入成功 
     */
    Q_INVOKABLE bool loadInsFile(const QString& path);
    Q_INVOKABLE void saveLogFile(const QString & path);

    Q_INVOKABLE void scheduler(int timeLen);//调度
    
    Q_INVOKABLE inline bool isRunning()
    {
        return m_scheduler->isRunning();
    }
    inline QString curRunProc()
    {
        int cur=m_scheduler->curRunPorc();
        if(cur==0)
            return QString();
        return QString("P%1").arg(cur);
    }
    void updateAll();
signals:
    void over();//调度完成
    void tick();//一个时间片完成
    void doSheduler();//开始调度
public slots:
};

#endif // SIMULATOR