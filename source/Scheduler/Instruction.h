#ifndef INSTRUCTION_H
#define INSTRUCTION_H

enum class InstructionType
{
    CPU,INPUT,OUTPUT,WAIT
};

struct Instruction
{
    InstructionType iType;
    int runTime;    
};



#endif//INSTRUCTION_H