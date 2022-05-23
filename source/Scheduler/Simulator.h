#ifndef SIMULATOR
#define SIMULATOR
#include<QObject>
#include<QtQml>
#include"ModelPCBList.h"
class Scheduler;
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
private:
    static QObject* window;
    QObject* m_window;//qml window 对象，调用qml方法
    QThread* m_thread;//模拟线程
    Scheduler* m_scheduler;//调度器
    static Simulator* instence;
    //每一个队列的视图
    ModelPCBList m_modelReadyQue;
    ModelPCBList m_modelInputQue;
    ModelPCBList m_modelOutputQue;
    ModelPCBList m_modelWaitQue;
    ModelPCBList m_modelOverQue;
private:
    Simulator();
public:
    static void setWindow(QObject* window);
    static Simulator* getInstence();
    static void destroy();
    static void printLog(const QString& msg);//打印日志
    static void printInfo(const QString& info,const QString& color="blue");
    virtual ~Simulator();
    /**
     * @brief 载入指令 
     * 
     * @param path 
     * @return bool 返回 是否载入成功 
     */
    Q_INVOKABLE bool loadInsFile(const QString& path);
    inline ModelPCBList* getModelReadyQue()
    {
        return &m_modelReadyQue;
    }
    
public slots:
    void onUpdateModel(int model);
};

#endif // SIMULATOR