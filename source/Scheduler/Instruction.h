#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include<QString>
enum class InstructionType
{
    UNKNOW = -1,
    CPU,
    INPUT,
    OUTPUT,
    WAIT
};

struct Instruction
{
    InstructionType iType = Instruction::iType;
    int runTime = 0;
    static InstructionType CharToType(char c)
    {
        switch (c)
        {
        case 'C':
            return InstructionType::CPU;
        case 'I':
            return InstructionType::INPUT;
        case 'O':
            return InstructionType::OUTPUT;
        case 'W':
            return InstructionType::WAIT;
        default:
            return InstructionType::UNKNOW;
        }
    }
    QString toString()
    {
        QString ins="[ ";
        switch (iType)
        {
        case InstructionType::CPU:
            ins+="C : ";
            break;
        case InstructionType::INPUT:
            ins+="I : ";
            break;
        case InstructionType::OUTPUT:
            ins+="O : ";
            break;
        case InstructionType::WAIT:
            ins+="W : ";
            break;
        default:
            ins+="ERROR : ";
            break;
        }
        ins+=QString::number(runTime);
        ins+=" ]";
        return ins;   
    }
};

#endif // INSTRUCTION_H