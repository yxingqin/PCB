#include"PCB.h"

PCB::PCB():m_id(0),m_totalTime(0),m_curIns(0)
{
}
PCB::~PCB(){}

int PCB::tick(int time)
{
    return 0;
}
void PCB::addIns(const Instruction& ins)
{
    m_insList.append(ins);
}