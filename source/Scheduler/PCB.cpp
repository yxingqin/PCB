#include"PCB.h"

PCB::PCB(int id):m_id(id),m_totalTime(0),m_curInsIt(m_insList.begin())
{
}
PCB::~PCB(){}

int PCB::tick(int time)
{
    return 0;
}
void PCB::addIns(Instruction ins)
{
    //qDebug()<<"add ins"<<ins.toString();
    m_insList.push_back(ins);
    m_curInsIt=m_insList.begin();
}

QString PCB::toString()
{
    QString ret;
    ret.append(QString("pcbId: %1  ").arg(m_id));
    for(auto& it:m_insList)
    {
        ret.append(it.toString()+' ');
    }
    return ret;   
}