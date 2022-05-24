#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include<QString>
enum class InstructionType
{
    NONE = -1,//标记运行完毕 ,错误
    CPU,
    INPUT,
    OUTPUT,
    WAIT
};

struct Instruction
{
    InstructionType iType;
    int runTime;
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
            return InstructionType::NONE;
        }
    }
    char insTypeChar()const
    {
        switch (iType)
        {
        case InstructionType::CPU:
            return 'C';
        case InstructionType::INPUT:
            return  'I';
        case InstructionType::OUTPUT:
            return 'O';
        case InstructionType::WAIT:
            return 'W';
        default:
            return 'E';
        }
    }
    QString toString()const
    {
        QString ins="[ ";
        ins+=insTypeChar();
        ins+=" ";
        ins+=QString::number(runTime);
        ins+=" ]";
        return ins;   
    }
};

#endif // INSTRUCTION_H