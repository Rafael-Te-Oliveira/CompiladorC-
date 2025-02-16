#ifndef _ASSEMBLY_H_
#define _ASSEMBLY_H_

#include "cgen.h"

typedef enum
{
    ND,
    ADDI,
    SUBI,
    JP,
    JR,
    BEQ,
    BNE,
    BLT,
    BGT,
    BLE,
    BGE,
    LW,
    SW,
    JAL,
    OUT,
    IN,
    NOP,
    HLT,
    ADD,
    SUB,
    MUL,
    DIV,
    AND,
    OR,
    NOT,
    QNT = 62,
    PRG = 61
} InstructionKind;

typedef enum
{
    $zero,
    $t0,
    $t1,
    $t2,
    $t3,
    $t4,
    $t5,
    $t6,
    $t7,
    $t8,
    $t9,
    $t10,
    $t11,
    $t12,
    $t13,
    $t14,
    $t15,
    $t16,
    $t17,
    $t18,
    $t19,
    $t20,
    $t21,
    $t22,
    $t23,
    $a0,
    $a1,
    $a2,
    $ad,
    $v0,
    $sp,
    $ra
} Register;

typedef enum
{
    R,
    I,
    J,
    H
} Type;

typedef enum
{
    Inst,
    Lab
} LineKind;

typedef struct
{
    InstructionKind instKind;
    Register rs, rt, rd;
    int immediate, lineNo;
    Type type;
    LineKind lineKind;
    char *label;
} Instruction;

typedef struct InstructionListRec
{
    Instruction inst;
    struct InstructionListRec *next;
} *InstructionList;

typedef struct freeRegListRec
{
    Register r;
    struct freeRegListRec *prev;
    struct freeRegListRec *next;
} *freeRegList;

extern int *labelMap;

InstructionList assemblyGen(QuadList head);

int searchInstLineLabel(int labelNo);
#endif