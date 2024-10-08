#ifndef _BINARY_H_
#define _BINARY_H_

#include "assembly.h"

typedef struct{
    int opCode, rs, rt, rd, IMM;
    Type type;
} BinInstruction;

typedef struct BinaryListRec{
    BinInstruction bin;
    struct BinaryListRec * next; 
} * BinaryList;

void binaryGen(InstructionList instList);

#endif