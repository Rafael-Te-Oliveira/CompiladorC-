#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#define SIZE 211
#define SHIFT 4


static int hash (char* name, char* scope)
{ 
    int temp = 0;
    int i = 0;
    while (name[i] != '\0')
    { 
        temp = ((temp << SHIFT) + name[i]) % SIZE;
        i++;
    }
    i = 0;
    while (scope[i] != '\0')
    { 
        temp = ((temp << SHIFT) + scope[i]) % SIZE;
        i++;
    }
    return temp;
}

typedef struct LineListRec
{ 
    int lineno;
    struct LineListRec * next;
    
} * LineList;

typedef struct BucketListRec
{ 
     char* name;
     LineList lines;
     int memloc; 
     int nParam;
     int size;
	 char* scope;
	 char* typeID;
	 char* typeData; 
     int * instLine;
     struct BucketListRec * next;
     
 } * BucketList;

static BucketList hashTable[SIZE];

void stInsert( char * name, int lineno,int nParam, int memloc, char* scope, 
		char* typeID, char* typeData, int size)
{ 
 
    int h = hash(name, scope);
    BucketList l =  hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0)) 
            l = l->next;
    if (l == NULL) /* variable not yet in table */
    { 
        l = (BucketList) malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lines = (LineList) malloc(sizeof(struct LineListRec));
        l->lines->lineno = lineno;
        l->nParam = nParam;
        l->memloc = -1;
        l->size = size;
        l->lines->next = NULL;
        l->scope = scope;
        l->typeID = typeID;
        l->typeData = typeData;
        l->instLine = NULL;
        l->next = hashTable[h];
        hashTable[h] = l; 
    }
    else
    {
        LineList t = l->lines;
        while (t->next != NULL && t->lineno != lineno) 
            t = t->next;
        if(t->lineno != lineno){
        t->next = (LineList) malloc(sizeof(struct LineListRec));
        t->next->lineno = lineno;
        t->next->next = NULL; 
        } 
    }
} 

int statementFinder (char* name, char* scope){ // Procura na tabela e retorna se esta presente
    int h = hash(name, scope);    
    BucketList l =  hashTable[h];

    while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0))
        l = l->next;
    if (l == NULL) 
        return 0;
    else 
        return 1;
}


int statementFinderMemloc (char* name, char* scope)
{ 
  int h = hash(name, scope);	
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0))
        l = l->next;
  if (l == NULL) 
      return -1;
  else 
      return l->memloc;
}


char* statementFinderType (char* name, char* scope)
{ 
  int h = hash(name, scope);	
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0))
        l = l->next;
  if (l == NULL) 
      return "null";
  else 
      return l->typeData;
}

int statementFinderNparam (char *name, char *scope){ // Procura na tabela e retorna a quantidade de parametros da funcao
    int h = hash(name, scope);	
    BucketList l =  hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0)){
        l = l->next;
  }
  if (l == NULL) 
      return -1;
  else 
      return l->nParam;
}

int statementFinderSize(char *name, char *scope){ // Procura na tabela e retorna o tamanho do vetor
    int h = hash(name, scope);	
    BucketList l =  hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0)){
        l = l->next;
  }
    if (l == NULL) 
        return -1;
    else 
        return l->size;
}

int statementFinderInstLine(char * name){
    int h = hash(name, "global");
    BucketList l =  hashTable[h];

    while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp("global",l->scope) != 0))
        l = l->next;

    return * l->instLine;
}



void printSymTab(FILE * tabSim)
{ int i;
  fprintf(tabSim,"NOME          ESCOPO           TIPO_ID         TIPO_DATA         LINHAS\n");
  fprintf(tabSim,"--------      ------------     ------------    ------------      ------------\n");
  for (i=0;i<SIZE;++i)
  { if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
      { LineList t = l->lines;
        fprintf(tabSim,"%-14s  ",l->name);
		fprintf(tabSim,"%-14s  ",l->scope);
        fprintf(tabSim,"%-14s  ",l->typeID);
		fprintf(tabSim,"%-14s  ",l->typeData);	
        while (t != NULL)
        { fprintf(tabSim,"%4d ",t->lineno);
          t = t->next;
        }
        fprintf(tabSim,"\n");
        l = l->next;
      }
    }
  }
}

void insertMemoryData(char * name, char * scope, int location, int * instLine){
    int h = hash(name, scope);
    BucketList l =  hashTable[h];

    while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0))
        l = l->next;
    l->memloc = location;
    l->instLine = instLine;
}

void printMemInfo(FILE * memoryFile){
    fprintf(memoryFile,"  Local             Nome          Escopo         TipoDado         Tamanho        Linha da instrucao\n");
    fprintf(memoryFile,"---------       ------------    ----------      ----------      -----------     --------------------\n");
    // Print das infos da variaveis globais e locais
    for (int i=0;i<SIZE;++i){ 
        if (hashTable[i] != NULL){ 
            BucketList l = hashTable[i];
            while (l != NULL){
                if(l->memloc >= 0){
                    if(strcmp(l->scope,"global") != 0)
                        fprintf(memoryFile,"$sp+%-10d  ",l->memloc);
                    else 
                        fprintf(memoryFile,"%-14d  ",l->memloc);
                        fprintf(memoryFile,"%-14s  ",l->name);
                        fprintf(memoryFile,"%-14s  ",l->scope);
                        fprintf(memoryFile,"%-14s  ",l->typeData);
                        fprintf(memoryFile,"%-14d  ",l->size); 
                    if(l->instLine != NULL) fprintf(memoryFile,"%-14d  ",*l->instLine); 
                        fprintf(memoryFile,"\n");
                }
                l = l->next;
            }
        }
    }

    // Print da info das funcoes
    fprintf(memoryFile,"\n----------------------------------------------------------------------------------------------------\n\n");
    for (int i=0;i<SIZE;++i){ 
        if (hashTable[i] != NULL){ 
            BucketList l = hashTable[i];
            while (l != NULL){
                if(strcmp(l->typeID,"function") == 0){
                    fprintf(memoryFile,"---------       ");
                    fprintf(memoryFile,"%-14s  ",l->name);
                    fprintf(memoryFile,"%-14s  ",l->scope);
                    fprintf(memoryFile,"%-14s  ",l->typeData);
                    fprintf(memoryFile,"----------      "); 
                    fprintf(memoryFile,"%-14d  ",*l->instLine); 
                fprintf(memoryFile,"\n");
                }
                l = l->next;
            }
        }
    }
}
