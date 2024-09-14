#include "globals.h"
#include "assembly.h"
#include "cgen.h"
#include "symtab.h"

InstructionList instListHead = NULL;
freeRegList RegListHead = NULL;
freeRegList RegListTail = NULL;
Register argRegister;
int sPos;                  // Local do stack na memoria
int globalVarLocation = 0; // Onde colocar variaveis globais na memoria
int lineNo = 0;            // Numero da linha da instrucao Assembly
char *currentScope;
int argCounter;
int *labelMap;

int isNumber(char *str)
{ // Verifica se contem apenas digitos
    for (int i = 0; i < strlen(str); i++)
        if (isdigit(str[i]) == 0)
            return 0;

    return 1;
}

int searchInstLineLabel(int labelNo)
{ // Retorna o numero da instrucao referente ao label
    return labelMap[labelNo];
}

Register tempRegister(char *temp)
{
    if (strcmp(temp, "$t0") == 0)
        return $t0;
    else if (strcmp(temp, "$t1") == 0)
        return $t1;
    else if (strcmp(temp, "$t2") == 0)
        return $t2;
    else if (strcmp(temp, "$t3") == 0)
        return $t3;
    else if (strcmp(temp, "$t4") == 0)
        return $t4;
    else if (strcmp(temp, "$t5") == 0)
        return $t5;
    else if (strcmp(temp, "$t6") == 0)
        return $t6;
    else if (strcmp(temp, "$t7") == 0)
        return $t7;
    else if (strcmp(temp, "$t8") == 0)
        return $t8;
    else if (strcmp(temp, "$t9") == 0)
        return $t9;
    else if (strcmp(temp, "$t10") == 0)
        return $t10;
    else if (strcmp(temp, "$t11") == 0)
        return $t11;
    else if (strcmp(temp, "$t12") == 0)
        return $t12;
    else if (strcmp(temp, "$t13") == 0)
        return $t13;
    else if (strcmp(temp, "$t14") == 0)
        return $t14;
    else if (strcmp(temp, "$t15") == 0)
        return $t15;
    else if (strcmp(temp, "$t16") == 0)
        return $t16;
    else if (strcmp(temp, "$t17") == 0)
        return $t17;
    else if (strcmp(temp, "$t18") == 0)
        return $t18;
    else if (strcmp(temp, "$t19") == 0)
        return $t19;
    else if (strcmp(temp, "$t20") == 0)
        return $t20;
    else if (strcmp(temp, "$t21") == 0)
        return $t21;
    else if (strcmp(temp, "$t22") == 0)
        return $t22;
    else if (strcmp(temp, "$t23") == 0)
        return $t23;
    else
        return -1;
}

char *registerToString(Register r)
{
    char *reg = malloc(6 * (sizeof(char)));
    switch (r)
    {
    case $zero:
        strcpy(reg, "$zero");
        break;
    case $t0:
        strcpy(reg, "$t0");
        break;
    case $t1:
        strcpy(reg, "$t1");
        break;
    case $t2:
        strcpy(reg, "$t2");
        break;
    case $t3:
        strcpy(reg, "$t3");
        break;
    case $t4:
        strcpy(reg, "$t4");
        break;
    case $t5:
        strcpy(reg, "$t5");
        break;
    case $t6:
        strcpy(reg, "$t6");
        break;
    case $t7:
        strcpy(reg, "$t7");
        break;
    case $t8:
        strcpy(reg, "$t8");
        break;
    case $t9:
        strcpy(reg, "$t9");
        break;
    case $t10:
        strcpy(reg, "$t10");
        break;
    case $t11:
        strcpy(reg, "$t11");
        break;
    case $t12:
        strcpy(reg, "$t12");
        break;
    case $t13:
        strcpy(reg, "$t13");
        break;
    case $t14:
        strcpy(reg, "$t14");
        break;
    case $t15:
        strcpy(reg, "$t15");
        break;
    case $t16:
        strcpy(reg, "$t16");
        break;
    case $t17:
        strcpy(reg, "$t17");
        break;
    case $t18:
        strcpy(reg, "$t18");
        break;
    case $t19:
        strcpy(reg, "$t19");
        break;
    case $t20:
        strcpy(reg, "$t20");
        break;
    case $t21:
        strcpy(reg, "$t21");
        break;
    case $t22:
        strcpy(reg, "$t22");
        break;
    case $t23:
        strcpy(reg, "$t23");
        break;
    case $a0:
        strcpy(reg, "$a0");
        break;
    case $a1:
        strcpy(reg, "$a1");
        break;
    case $a2:
        strcpy(reg, "$a2");
        break;
    case $ad:
        strcpy(reg, "$ad");
        break;
    case $v0:
        strcpy(reg, "$v0");
        break;
    case $sp:
        strcpy(reg, "$sp");
        break;
    case $ra:
        strcpy(reg, "$ra");
        break;
    default:
        break;
    }
    return reg;
}

void freeRegInsert(Register r)
{
    freeRegList newUnusedReg = (freeRegList)malloc(sizeof(struct freeRegListRec));
    newUnusedReg->r = r;
    newUnusedReg->next = NULL;
    if (RegListHead == NULL)
    {
        newUnusedReg->prev = NULL;
        RegListHead = newUnusedReg;
        RegListTail = RegListHead;
    }
    else
    {
        RegListTail->next = newUnusedReg;
        newUnusedReg->prev = RegListTail;
        RegListTail = newUnusedReg;
    }
}

void freeRegRemove(Register r)
{
    freeRegList unusedReg = RegListHead;
    while (unusedReg != NULL && unusedReg->r != r)
        unusedReg = unusedReg->next;
    if (unusedReg == NULL)
        ;
    else if (unusedReg == RegListHead)
    {
        if (unusedReg == RegListTail)
        {
            RegListHead = NULL;
            RegListTail = NULL;
        }
        else
        {
            RegListHead = RegListHead->next;
            RegListHead->prev = NULL;
        }
    }
    else if (unusedReg == RegListTail)
    {
        RegListTail = RegListTail->prev;
        RegListTail->next = NULL;
    }
    else
    {
        unusedReg->prev->next = unusedReg->next;
        unusedReg->next->prev = unusedReg->prev;
    }
    free(unusedReg);
}

void assemblyInsert(InstructionKind instKind, Register rs, Register rt, Register rd, int immediate, int lineNo, Type tp, LineKind lineKind, char *lab)
{ // Insere uma instrucao ou label Assembly
    Instruction newInst;
    newInst.instKind = instKind;
    newInst.rs = rs;
    newInst.rt = rt;
    newInst.rd = rd;
    newInst.immediate = immediate;
    newInst.lineNo = lineNo;
    newInst.type = tp;
    newInst.lineKind = lineKind;
    newInst.label = malloc(strlen(lab) * sizeof(char));
    strcpy(newInst.label, lab);
    InstructionList newAssembly = (InstructionList)malloc(sizeof(struct InstructionListRec));
    newAssembly->inst = newInst;
    newAssembly->next = NULL;
    if (instListHead == NULL)
        instListHead = newAssembly;
    else
    {
        InstructionList a = instListHead;
        while (a->next != NULL)
            a = a->next;
        a->next = newAssembly;
    }
}

void assemblyQuadFUN(Quad q)
{
    int *instLine = malloc(sizeof(int));
    *instLine = lineNo;                                    // numero da linha da funçao
    insertMemoryData(q.arg2, "global", -1, instLine);      // guarda na memória
    assemblyInsert(0, 0, 0, 0, 0, lineNo, 0, Lab, q.arg2); // Insere um label
    sPos = 0;                                              // Inicia topo da pilha
    currentScope = q.arg2;                                 // Armazena escopo
    argRegister = $a0;                                     // Inicializa o registrador de argumento como $a0
    argCounter = statementFinderNparam(q.arg2, "global");  // Guarda numero de argumentos da funcao
}

void assemblyQuadARG(Quad q)
{
    insertMemoryData(q.arg2, q.arg3, sPos, NULL);                       // Insere na area local da memoria o numero inteiro
    assemblyInsert(SW, argRegister, $sp, 0, sPos, lineNo, I, Inst, ""); // Instrucao de SW do argumento
    sPos++;                                                             // Incrementa pilha
    lineNo++;                                                           // Incrementa numero da linha de instrucao
    argRegister++;                                                      // Proximo reg de argumento
    argCounter--;                                                       // Decrementa quantidade de argumentos restantes
    if (argCounter == 0)                                                // Se era ultimo
        argRegister = $a0;                                              // Reseta para o primeiro
}

void assemblyQuadPARAM(Quad q)
{
    assemblyInsert(ADD, argRegister, tempRegister(q.arg1), $zero, 0, lineNo, R, Inst, ""); // Move o conteudo do temp para o argumento
    lineNo++;
    argRegister++;
    freeRegRemove(tempRegister(q.arg1)); // Remove o reg temp da lista de livres
}

void assemblyQuadALLOC(Quad q)
{
    if (strcmp(q.arg2, "global") == 0)
    {                                                              // Se variavel for global
        insertMemoryData(q.arg1, q.arg2, globalVarLocation, NULL); // Aloca espaço na memoria global
        globalVarLocation += statementFinderSize(q.arg1, q.arg2);  // Soma a proxima posicao a ser inserida na global
    }
    else
    {                                                 // Se variavel for local
        insertMemoryData(q.arg1, q.arg2, sPos, NULL); // Aloca espaco na memoria na area local
        sPos += statementFinderSize(q.arg1, q.arg2);  // Soma o topo da pilha
    }
}

void assemblyQuadCALL(Quad q)
{
    if (strcmp(q.arg2, "input") == 0)
    {
        assemblyInsert(IN, tempRegister(q.arg1), tempRegister(q.arg1), tempRegister(q.arg1), 0, lineNo, R, Inst, "");
        lineNo++;
        freeRegInsert(tempRegister(q.arg1));
    }
    else if (strcmp(q.arg2, "output") == 0)
    {
        assemblyInsert(OUT, $a0, 0, 0, 0, lineNo, R, Inst, "");
        lineNo++;
        argRegister = $a0;
    }
    else
    {
        freeRegList unusedRegister = RegListHead; // Variavel para iterar lista de registradores livres
        while (unusedRegister != NULL)
        {
            assemblyInsert(SW, unusedRegister->r, $sp, 0, sPos, lineNo, I, Inst, ""); // Insere na pilha aqueles que nao foram usados
            lineNo++;
            sPos++;                                // Incrementa pilha
            unusedRegister = unusedRegister->next; // proximo registrador
        }

        if (strcmp(currentScope, "main") != 0)
        {                                                               // Se recursao, empilha $ra
            assemblyInsert(SW, $ra, $sp, 0, sPos, lineNo, I, Inst, ""); // SW endereco de retorno salvo em $ra da chamada anterior
            sPos++;                                                     // Incrementa pilha
            lineNo++;
        }

        assemblyInsert(ADDI, $sp, $sp, 0, sPos, lineNo, I, Inst, ""); // Soma o inicio da proxima funcao na memoria
        lineNo++;
        argRegister = $a0;
        assemblyInsert(JAL, 0, 0, 0, 0, lineNo, J, Inst, q.arg2); // JAL para funcao
        lineNo++;
        assemblyInsert(SUBI, $sp, $sp, 0, sPos, lineNo, I, Inst, ""); // SUBI retorna $sp pro escopo anterior
        lineNo++;
        if (strcmp(currentScope, "main") != 0)
        {                                                               // Se recursao
            sPos--;                                                     // Desempilha o endereco de retorno da pilha
            assemblyInsert(LW, $ra, $sp, 0, sPos, lineNo, I, Inst, ""); // Retorna o endereco de retorno da chamada atual para o registrador $ra
            lineNo++;
        }

        unusedRegister = RegListTail;
        while (unusedRegister != NULL)
        {
            sPos--;                                                                   // Desempilha o registrador da pilha
            assemblyInsert(LW, unusedRegister->r, $sp, 0, sPos, lineNo, I, Inst, ""); // LW pilha os registradores
            lineNo++;
            unusedRegister = unusedRegister->prev;
        }

        if (strcmp(statementFinderType(q.arg2, "global"), "integer") == 0)
        {                                                                                  // Se retorno int, salva conteudo de $v0 no temp
            assemblyInsert(ADD, tempRegister(q.arg1), $v0, $zero, 0, lineNo, R, Inst, ""); // Move o valor retornado ao temp
            lineNo++;
            freeRegInsert(tempRegister(q.arg1));
        }
    }
}

void assemblyQuadRET(Quad q)
{
    assemblyInsert(ADD, $v0, tempRegister(q.arg1), $zero, 0, lineNo, R, Inst, ""); // move o conteudo do temp para o registrador $v0
    lineNo++;
    assemblyInsert(JR, $ra, 0, 0, 0, lineNo, R, Inst, ""); // JR para o endereco presente em $ra
    lineNo++;
    freeRegRemove(tempRegister(q.arg1));
}

void assemblyQuadEND(Quad q)
{
    if (strcmp(q.arg1, "main") != 0)
    {                                                          // Se nao for main
        assemblyInsert(JR, $ra, 0, 0, 0, lineNo, R, Inst, ""); // JR para endereco em $ra
        lineNo++;
    }
}

void assemblyQuadLOAD(Quad q)
{
    if (isNumber(q.arg2))
    {                                                                                            // Se constante
        assemblyInsert(ADDI, tempRegister(q.arg1), $zero, 0, atoi(q.arg2), lineNo, I, Inst, ""); // ADDI constante
        lineNo++;
    }
    else
    { // Se variavel na memoria
        int location;
        if (statementFinder(q.arg2, "global") == 1)
        {
            location = statementFinderMemloc(q.arg2, "global");
            if (strcmp(q.arg3, "-") != 0)
            {                                                                                                     // Se for um vetor
                assemblyInsert(LW, tempRegister(q.arg1), tempRegister(q.arg3), 0, location, lineNo, I, Inst, ""); // LW da posicao em location + o indice
                freeRegRemove(tempRegister(q.arg3));
            }
            else                                                                                   // Se nao for um vetor
                assemblyInsert(LW, tempRegister(q.arg1), $zero, 0, location, lineNo, I, Inst, ""); // LW da posicao em location
            lineNo++;
        }
        else
        { // Se variavel local
            location = statementFinderMemloc(q.arg2, currentScope);
            if (strcmp(q.arg3, "-") != 0)
            { // Se for um vetor
                if (strcmp(statementFinderType(q.arg2, currentScope), "vector arg") == 0)
                {                                                                                    // Se for um vetor como argumento
                    assemblyInsert(LW, tempRegister(q.arg1), $sp, 0, location, lineNo, I, Inst, ""); // LW endereco base vetor
                    lineNo++;
                    assemblyInsert(ADD, tempRegister(q.arg1), tempRegister(q.arg1), tempRegister(q.arg3), 0, lineNo, R, Inst, ""); // ADD endereco base do vetor + indice
                    lineNo++;
                    assemblyInsert(LW, tempRegister(q.arg1), tempRegister(q.arg1), 0, 0, lineNo, I, Inst, ""); // LW conteudo do vetor naquele indice
                }
                else
                {                                                                                                 // Se for um vetor local
                    assemblyInsert(ADD, tempRegister(q.arg3), tempRegister(q.arg3), $sp, 0, lineNo, R, Inst, ""); // ADD endereco do vetor
                    lineNo++;
                    assemblyInsert(LW, tempRegister(q.arg1), tempRegister(q.arg3), 0, location, lineNo, I, Inst, ""); // LW valor no vetor no indice salvo na memoria
                }
                freeRegRemove(tempRegister(q.arg3));
            }
            else                                                                                 // Se nao for um vetor
                assemblyInsert(LW, tempRegister(q.arg1), $sp, 0, location, lineNo, I, Inst, ""); // LW dado daquele endereco
            lineNo++;
        }
    }
    freeRegInsert(tempRegister(q.arg1));
}

void assemblyQuadSTORE(Quad q)
{
    int location;
    if (statementFinder(q.arg1, "global") == 1)
    {
        location = statementFinderMemloc(q.arg1, "global");
        if (strcmp(q.arg3, "-") != 0)
        {                                                                                                     // Se for um vetor
            assemblyInsert(SW, tempRegister(q.arg2), tempRegister(q.arg3), 0, location, lineNo, I, Inst, ""); // SW posicao em location + indice do vetor
            freeRegRemove(tempRegister(q.arg3));
        }
        else                                                                                   // Se nao for um vetor
            assemblyInsert(SW, tempRegister(q.arg2), $zero, 0, location, lineNo, I, Inst, ""); // SW da posicao em location
        lineNo++;
    }
    else
    {
        location = statementFinderMemloc(q.arg1, currentScope);
        if (strcmp(q.arg3, "-") != 0)
        { // Se for um vetor
            if (strcmp(statementFinderType(q.arg1, currentScope), "vector arg") == 0)
            {                                                                   // Se for um vetor como argumento
                assemblyInsert(LW, $ad, $sp, 0, location, lineNo, I, Inst, ""); // LW carrega o endereco base desse vetor em $ad
                lineNo++;
                assemblyInsert(ADD, $ad, $ad, tempRegister(q.arg3), 0, lineNo, R, Inst, ""); // ADD endereco base do vetor + indice
                lineNo++;
                assemblyInsert(SW, tempRegister(q.arg2), $ad, 0, 0, lineNo, I, Inst, ""); // SW conteudo do vetor naquele indice
            }
            else
            {                                                                                                 // Se for um vetor alocado local
                assemblyInsert(ADD, tempRegister(q.arg3), tempRegister(q.arg3), $sp, 0, lineNo, R, Inst, ""); // ADD endereco indice deslocado de $sp
                lineNo++;
                assemblyInsert(SW, tempRegister(q.arg2), tempRegister(q.arg3), 0, location, lineNo, I, Inst, ""); // SW temp em um vetor no indice endereco base + indice + $sp
            }
            freeRegRemove(tempRegister(q.arg3));
        }
        else                                                                                 // Se nao for um vetor
            assemblyInsert(SW, tempRegister(q.arg2), $sp, 0, location, lineNo, I, Inst, ""); // SW para guardar um dado naquele endereco
        lineNo++;
    }
    freeRegRemove(tempRegister(q.arg2));
}

void assemblyQuadASSIGN(Quad q)
{
    assemblyInsert(ADD, tempRegister(q.arg1), tempRegister(q.arg2), $zero, 0, lineNo, R, Inst, "");
    lineNo++;
    freeRegRemove(tempRegister(q.arg2));
}

void assemblyQuadLABEL(Quad q)
{
    char *labNo = &q.arg1[1];
    labelMap[atoi(labNo)] = lineNo;
    assemblyInsert(0, 0, 0, 0, 0, lineNo, 0, Lab, q.arg1);
}

void assemblyQuadGOTO(Quad q)
{
    assemblyInsert(JP, 0, 0, 0, 0, lineNo, J, Inst, q.arg1);
    lineNo++;
}

void assemblyQuadIFF(Quad q)
{
    assemblyInsert(BNE, tempRegister(q.arg1), $zero, 0, 0, lineNo, I, Inst, q.arg2);
    lineNo++;
    freeRegRemove(tempRegister(q.arg1));
}

void assemblyQuadArit(Quad q)
{
    InstructionKind operation;
    if (strcmp(q.op, "ADD") == 0)
        operation = ADD;
    else if (strcmp(q.op, "SUB") == 0)
        operation = SUB;
    else if (strcmp(q.op, "MUL") == 0)
        operation = MUL;
    else
        operation = DIV;
    assemblyInsert(operation, tempRegister(q.arg1), tempRegister(q.arg2), tempRegister(q.arg3), 0, lineNo, R, Inst, "");
    lineNo++;
    freeRegRemove(tempRegister(q.arg2));
    freeRegRemove(tempRegister(q.arg3));
    freeRegInsert(tempRegister(q.arg1));
}

void assemblyQuadEQ(Quad q, Quad branchInfo)
{
    assemblyInsert(BNE, tempRegister(q.arg2), tempRegister(q.arg3), 0, 0, lineNo, I, Inst, branchInfo.arg2);
    lineNo++;
    freeRegRemove(tempRegister(q.arg2));
    freeRegRemove(tempRegister(q.arg3));
}

void assemblyQuadNEQ(Quad q, Quad branchInfo)
{
    assemblyInsert(BEQ, tempRegister(q.arg2), tempRegister(q.arg3), 0, 0, lineNo, I, Inst, branchInfo.arg2);
    lineNo++;
    freeRegRemove(tempRegister(q.arg2));
    freeRegRemove(tempRegister(q.arg3));
}

void assemblyQuadLT(Quad q, Quad branchInfo)
{
    assemblyInsert(BGE, tempRegister(q.arg2), tempRegister(q.arg3), 0, 0, lineNo, I, Inst, branchInfo.arg2);
    lineNo++;
    freeRegRemove(tempRegister(q.arg2));
    freeRegRemove(tempRegister(q.arg3));
}

void assemblyQuadGT(Quad q, Quad branchInfo)
{
    assemblyInsert(BLE, tempRegister(q.arg2), tempRegister(q.arg3), 0, 0, lineNo, I, Inst, branchInfo.arg2);
    lineNo++;
    freeRegRemove(tempRegister(q.arg2));
    freeRegRemove(tempRegister(q.arg3));
}

void assemblyQuadLET(Quad q, Quad branchInfo)
{
    assemblyInsert(BGT, tempRegister(q.arg2), tempRegister(q.arg3), 0, 0, lineNo, I, Inst, branchInfo.arg2);
    lineNo++;
    freeRegRemove(tempRegister(q.arg2));
    freeRegRemove(tempRegister(q.arg3));
}

void assemblyQuadGET(Quad q, Quad branchInfo)
{
    assemblyInsert(BLT, tempRegister(q.arg2), tempRegister(q.arg3), 0, 0, lineNo, I, Inst, branchInfo.arg2);
    lineNo++;
    freeRegRemove(tempRegister(q.arg2));
    freeRegRemove(tempRegister(q.arg3));
}

void assemblyQuadHLT(Quad q)
{
    assemblyInsert(HLT, 0, 0, 0, 0, lineNo, H, Inst, "");
    lineNo++;
}

void printAssembly(FILE *assemblyFile)
{
    InstructionList i = instListHead;
    while (i != NULL)
    {
        switch (i->inst.lineKind)
        {
        case Inst:
            switch (i->inst.instKind)
            {
            case ADD:
                fprintf(assemblyFile, "%d %s %s, %s, %s\n", i->inst.lineNo, "ADD", registerToString(i->inst.rs), registerToString(i->inst.rt), registerToString(i->inst.rd));
                break;
            case SUB:
                fprintf(assemblyFile, "%d %s %s, %s, %s\n", i->inst.lineNo, "SUB", registerToString(i->inst.rs), registerToString(i->inst.rt), registerToString(i->inst.rd));
                break;
            case MUL:
                fprintf(assemblyFile, "%d %s %s, %s, %s\n", i->inst.lineNo, "MUL", registerToString(i->inst.rs), registerToString(i->inst.rt), registerToString(i->inst.rd));
                break;
            case DIV:
                fprintf(assemblyFile, "%d %s %s, %s, %s\n", i->inst.lineNo, "DIV", registerToString(i->inst.rs), registerToString(i->inst.rt), registerToString(i->inst.rd));
                break;
            case NOT:
                fprintf(assemblyFile, "%d %s %s %s\n", i->inst.lineNo, "NOT", registerToString(i->inst.rs), registerToString(i->inst.rt));
                break;
            case AND:
                fprintf(assemblyFile, "%d %s %s, %s, %s\n", i->inst.lineNo, "AND", registerToString(i->inst.rs), registerToString(i->inst.rt), registerToString(i->inst.rd));
                break;
            case OR:
                fprintf(assemblyFile, "%d %s %s, %s, %s\n", i->inst.lineNo, "OR", registerToString(i->inst.rs), registerToString(i->inst.rt), registerToString(i->inst.rd));
                break;
            case BEQ:
                fprintf(assemblyFile, "%d %s %s, %s, %s\n", i->inst.lineNo, "BEQ", registerToString(i->inst.rs), registerToString(i->inst.rt), i->inst.label);
                break;
            case BNE:
                fprintf(assemblyFile, "%d %s %s, %s, %s\n", i->inst.lineNo, "BNE", registerToString(i->inst.rs), registerToString(i->inst.rt), i->inst.label);
                break;
            case BLT:
                fprintf(assemblyFile, "%d %s %s, %s, %s\n", i->inst.lineNo, "BLT", registerToString(i->inst.rs), registerToString(i->inst.rt), registerToString(i->inst.rd));
                break;
            case BGT:
                fprintf(assemblyFile, "%d %s %s, %s, %s\n", i->inst.lineNo, "BGT", registerToString(i->inst.rs), registerToString(i->inst.rt), registerToString(i->inst.rd));
                break;
            case BLE:
                fprintf(assemblyFile, "%d %s %s, %s, %s\n", i->inst.lineNo, "BLE", registerToString(i->inst.rs), registerToString(i->inst.rt), registerToString(i->inst.rd));
                break;
            case BGE:
                fprintf(assemblyFile, "%d %s %s, %s, %s\n", i->inst.lineNo, "BGE", registerToString(i->inst.rs), registerToString(i->inst.rt), registerToString(i->inst.rd));
                break;
            case JR:
                fprintf(assemblyFile, "%d %s %s\n", i->inst.lineNo, "JR", registerToString(i->inst.rs));
                break;
            case IN:
                fprintf(assemblyFile, "%d %s %s\n", i->inst.lineNo, "IN", registerToString(i->inst.rs));
                break;
            case OUT:
                fprintf(assemblyFile, "%d %s %s\n", i->inst.lineNo, "OUT", registerToString(i->inst.rs));
                break;
            case ADDI:
                fprintf(assemblyFile, "%d %s %s, %s, %d\n", i->inst.lineNo, "ADDI", registerToString(i->inst.rs), registerToString(i->inst.rt), i->inst.immediate);
                break;
            case SUBI:
                fprintf(assemblyFile, "%d %s %s, %s, %d\n", i->inst.lineNo, "SUBI", registerToString(i->inst.rs), registerToString(i->inst.rt), i->inst.immediate);
                break;
            case LW:
                fprintf(assemblyFile, "%d %s %s, %s, %d\n", i->inst.lineNo, "LW", registerToString(i->inst.rs), registerToString(i->inst.rt), i->inst.immediate);
                break;
            case SW:
                fprintf(assemblyFile, "%d %s %s, %s, %d\n", i->inst.lineNo, "SW", registerToString(i->inst.rs), registerToString(i->inst.rt), i->inst.immediate);
                break;
            case JP:
                fprintf(assemblyFile, "%d %s %s\n", i->inst.lineNo, "JP", i->inst.label);
                break;
            case JAL:
                fprintf(assemblyFile, "%d %s %s\n", i->inst.lineNo, "JAL", i->inst.label);
                break;
            case NOP:
                fprintf(assemblyFile, "%d %s\n", i->inst.lineNo, "NOP");
                break;
            case HLT:
                fprintf(assemblyFile, "%d %s", i->inst.lineNo, "HLT");
                break;
            default:
                break;
            }
            break;

        case Lab:
            fprintf(assemblyFile, ".%s\n", i->inst.label);
            break;

        default:
            break;
        }
        i = i->next;
    }
}

void printLabelInfo(FILE *memoryFile)
{
    for (int i = 0; i < nlabel; i++)
    {
        fprintf(memoryFile, "---------       ");
        fprintf(memoryFile, "L%-13d  ", i);
        fprintf(memoryFile, "----------      label           ----------      ");
        fprintf(memoryFile, "%d\n", labelMap[i]);
    }
}

InstructionList assemblyGen(QuadList head)
{
    labelMap = malloc(nlabel * sizeof(int));
    sPos = globalSize;

    assemblyInsert(ADDI, $sp, $zero, 0, sPos, lineNo, I, Inst, "");
    lineNo++;
    assemblyInsert(JP, 0, 0, 0, 0, lineNo, J, Inst, "main");
    lineNo++;

    QuadList q = head;
    while (q != NULL)
    {
        if (strcmp(q->quad.op, "FUN") == 0)
            assemblyQuadFUN(q->quad);
        else if (strcmp(q->quad.op, "ARG") == 0)
            assemblyQuadARG(q->quad);
        else if (strcmp(q->quad.op, "PARAM") == 0)
            assemblyQuadPARAM(q->quad);
        else if (strcmp(q->quad.op, "ALLOC") == 0)
            assemblyQuadALLOC(q->quad);
        else if (strcmp(q->quad.op, "CALL") == 0)
            assemblyQuadCALL(q->quad);
        else if (strcmp(q->quad.op, "RET") == 0)
            assemblyQuadRET(q->quad);
        else if (strcmp(q->quad.op, "END") == 0)
            assemblyQuadEND(q->quad);
        else if (strcmp(q->quad.op, "LOAD") == 0)
            assemblyQuadLOAD(q->quad);
        else if (strcmp(q->quad.op, "STORE") == 0)
            assemblyQuadSTORE(q->quad);
        else if (strcmp(q->quad.op, "ASSIGN") == 0)
            assemblyQuadASSIGN(q->quad);
        else if (strcmp(q->quad.op, "LABEL") == 0)
            assemblyQuadLABEL(q->quad);
        else if (strcmp(q->quad.op, "GOTO") == 0)
            assemblyQuadGOTO(q->quad);
        else if (strcmp(q->quad.op, "IFF") == 0)
            assemblyQuadIFF(q->quad);
        else if (strcmp(q->quad.op, "ADD") == 0)
            assemblyQuadArit(q->quad);
        else if (strcmp(q->quad.op, "SUB") == 0)
            assemblyQuadArit(q->quad);
        else if (strcmp(q->quad.op, "MUL") == 0)
            assemblyQuadArit(q->quad);
        else if (strcmp(q->quad.op, "DIV") == 0)
            assemblyQuadArit(q->quad);
        else if (strcmp(q->quad.op, "LT") == 0)
        {
            assemblyQuadLT(q->quad, q->next->quad);
            q = q->next;
        }
        else if (strcmp(q->quad.op, "GT") == 0)
        {
            assemblyQuadGT(q->quad, q->next->quad);
            q = q->next;
        }
        else if (strcmp(q->quad.op, "LET") == 0)
        {
            assemblyQuadLET(q->quad, q->next->quad);
            q = q->next;
        }
        else if (strcmp(q->quad.op, "GET") == 0)
        {
            assemblyQuadGET(q->quad, q->next->quad);
            q = q->next;
        }
        else if (strcmp(q->quad.op, "EQ") == 0)
        {
            assemblyQuadEQ(q->quad, q->next->quad);
            q = q->next;
        }
        else if (strcmp(q->quad.op, "NEQ") == 0)
        {
            assemblyQuadNEQ(q->quad, q->next->quad);
            q = q->next;
        }
        else if (strcmp(q->quad.op, "HLT") == 0)
            assemblyQuadHLT(q->quad);
        q = q->next;
    }

    FILE *codefile = fopen("output/assembly.txt", "w+");
    printAssembly(codefile);
    fclose(codefile);

    FILE *memoryFile = fopen("output/memory.txt", "w+");
    printMemInfo(memoryFile);
    printLabelInfo(memoryFile);
    fclose(memoryFile);

    return instListHead;
}