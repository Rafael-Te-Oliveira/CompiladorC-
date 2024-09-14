#include "globals.h"
#include "binary.h"
#include "symtab.h"

BinaryList binListHead = NULL;                                     

void decimalToBinary(int number, int bitNo, FILE * binFile){   // Converte decimal em binario
    char * binary = malloc(bitNo * sizeof(char));                   
    for(int i = bitNo - 1; i >= 0; i--, number = number / 2){       
        int bit = number % 2;                                       
        binary[i] = bit + '0';                                     
    }
    binary[bitNo] = '\0';

    fprintf(binFile,"%s",binary);
    free(binary);                                                   
}

void binaryInsert(int opCode, int rs, int rt, int rd, int IMM, Type type){     
    BinInstruction newInst;
    if (opCode>17) {
        newInst.opCode = 0;
        newInst.IMM = opCode%18; //imediato referencia a operaçao
    }
    else {
        newInst.opCode = opCode;
        newInst.IMM = IMM;
    }
    newInst.rs = rs;
    newInst.rt = rt;
    newInst.rd = rd;
    newInst.type = type;
    BinaryList newBinary = (BinaryList) malloc(sizeof(struct BinaryListRec));
    newBinary->bin = newInst;
    newBinary->next = NULL;
    if(binListHead == NULL) 
        binListHead = newBinary;
    else{
        BinaryList aux = binListHead;
        while(aux->next != NULL)  aux = aux->next;
        aux->next = newBinary;
    }
}

void binaryTypeI(Instruction inst){                                                    
    if(inst.instKind == BEQ || inst.instKind == BNE 
    || inst.instKind == BLT || inst.instKind == BGT
    || inst.instKind == BLE || inst.instKind == BGE){                                      // Se a instrucao for branch
        int instLine = searchInstLineLabel(atoi(&inst.label[1]));                          // procura a linha de instrucao
        binaryInsert(inst.instKind, inst.rs, inst.rt, inst.rd, instLine, inst.type);       
    }
    else                                                                                    // Caso contrario
        binaryInsert(inst.instKind, inst.rs, inst.rt, inst.rd, inst.immediate, inst.type);  
}

void binaryTypeJ(Instruction inst){                                                
    int instLine;                                                                       // Numero da linha da instrucao que referencia funcao ou label
    if(inst.instKind == JP){                                                            // Se instrucao Jump
        if(strcmp(inst.label,"main") == 0)                                             
            instLine = statementFinderInstLine(inst.label);                             // Procura o numero da linha main
        else                                                                           
            instLine = searchInstLineLabel(atoi(&inst.label[1]));                       // Procura o numero da label no labelMap
        binaryInsert(inst.instKind, inst.rs, inst.rt, inst.rd, instLine, inst.type);    
    }
    else{                                                                               // Se for JAL
        instLine = statementFinderInstLine(inst.label);                                 // Procura o numero da linha da funcao
        binaryInsert(inst.instKind, inst.rs, inst.rt, inst.rd, instLine, inst.type);    
    }
}

void printBinary(FILE * binFile){                                               
    BinaryList b = binListHead;
    while(b != NULL){
        switch(b->bin.type){
            case R:
                decimalToBinary(b->bin.opCode, 6, binFile);
                decimalToBinary(b->bin.rs, 5, binFile);
                decimalToBinary(b->bin.rt, 5, binFile);
                decimalToBinary(b->bin.rd, 5, binFile);
                decimalToBinary(b->bin.IMM, 11, binFile);
                break;
            case I:
                decimalToBinary(b->bin.opCode, 6, binFile);
                decimalToBinary(b->bin.rs, 5, binFile);
                decimalToBinary(b->bin.rt, 5, binFile);
                decimalToBinary(b->bin.IMM, 16, binFile);
                break;
            case J:
                decimalToBinary(b->bin.opCode, 6, binFile);
                decimalToBinary(b->bin.IMM, 26, binFile);
                break;
            case H:
                decimalToBinary(b->bin.opCode, 6, binFile);
                decimalToBinary(0, 26, binFile);
                break;
        }
        fprintf(binFile,"\n");
        b = b->next;
    }
}

void binaryGen(InstructionList instListHead){                                                                             
    InstructionList i = instListHead;

    while(i != NULL){
        if(i->inst.lineKind == Inst){
            switch(i->inst.type){
                case R:
                    binaryInsert(i->inst.instKind, i->inst.rs, i->inst.rt, i->inst.rd, i->inst.immediate, i->inst.type);
                    break;
                
                case I:
                    binaryTypeI(i->inst);
                    break;
                
                case J:
                    binaryTypeJ(i->inst);
                    break;
                
                case H:
                    binaryInsert(i->inst.instKind, i->inst.rs, i->inst.rt, i->inst.rd, i->inst.immediate, i->inst.type);
                    break;

                default:
                    break;
            }
        }
        i = i->next;
    }

    FILE * codefile = fopen("Output/codigoBinario.txt", "w+");
    printBinary(codefile);
    fclose(codefile);
}