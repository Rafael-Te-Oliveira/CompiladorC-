#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef YYPARSER

#include "parser.tab.h"

#define ENDFILE 0

#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXRESERVED 6

extern FILE* source; 
extern FILE* listing; 
extern FILE* code; 
extern FILE* arvSin;
extern FILE* tabSim;


extern int lineno; 

typedef int TokenType;

typedef enum
{
	statementK, expressionK 
} NodeKind;

typedef enum
{
	ifK, whileK, assignK, variableK, functionK, callK, returnK, numberK

} StatementKind;

typedef enum
{
	operationK, constantK, idK, vectorK, vectorIdK, typeK

} ExpressionIdentifier;

typedef enum
{

	voidK, integerK, arrayK, booleanK
	
} ExpressionType;


#define MAXCHILDREN 3


typedef struct treeNode
{ 
	 struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     NodeKind nodekind;

     union 
     { 
		StatementKind stmt; 
        ExpressionIdentifier exp;
     } kind;

     struct 
     { 
	    TokenType op;
        int val;
        int len;
        char* name; 
        char* scope;	
     } attr;

     ExpressionType type;
} TreeNode;

extern int Error; 

