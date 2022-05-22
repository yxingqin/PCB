#include"PCB.h"

PCB::PCB(int id):m_id(id),m_totalTime(0),m_curIns(0)
{
}
PCB::~PCB(){}

int PCB::tick(int time)
{
    return 0;
}
void PCB::addIns(Instruction&& ins)
{
    m_insList.push_back(ins);
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