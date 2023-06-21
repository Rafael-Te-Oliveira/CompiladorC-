#include "globals.h"
#include "symtab.h"
#include "analyze.h"

/* Contador de localizacao */
static int location = 0;
int numParam = 0;
/* Percorre arvore */
static void traverse(TreeNode *t,void (*preProc)(TreeNode *), void (*postProc)(TreeNode *))
{

    if (t != NULL)
    {
        preProc(t);
        {
            int i;
            for (i = 0; i < MAXCHILDREN; i++)
                traverse(t->child[i], preProc, postProc);
        }
        postProc(t);
        traverse(t->sibling, preProc, postProc);
    }
}

/* Conta paramentros */
int paramCounter(TreeNode *t)  // Funcao para contar os parametros
{

    int tmpcount = 0;

    t = t->child[0];

    while(t != NULL)
    {
        tmpcount++;
        t = t->sibling;
    }
    return tmpcount;
}

/* Pre-ordem e pos-ordem */
static void nullProc(TreeNode *t)
{
    if (t == NULL)
        return;
    else
        return;
}

/* Printa erro semantico no console */
static void printSemanticError(TreeNode *tree)
{
    fprintf(listing, "ERRO SEMÂNTICO: %s LINHA: %d\n", tree->attr.name, tree->lineno);
    Error = TRUE;
}

/* Insere na tabela de simbolos*/
static void insertNode(TreeNode *t)
{

    switch (t->nodekind)
    {
    case statementK:
        switch (t->kind.stmt){

        case variableK:
            if (statementFinderMemloc(t->attr.name, t->attr.scope) == -1 && statementFinderMemloc(t->attr.name, "global") == -1){
                if(t->type != voidK){
                    if(t->type == arrayK){
                    stInsert(t->attr.name, t->lineno, 0, location++, t->attr.scope, "variable", "vector", t->attr.len); // Nova def de variavel
                    }
                    else
                                stInsert(t->attr.name, t->lineno,location++, 0, t->attr.scope, "variable", "integer", 1); // Nova def de variavel
                        }
                        else
                            printSemanticError(t); //Variavel declarada como void 
                    } 
                    else
                        printSemanticError(t); //Variavel ja declarada 
                    break;
        
        case paramK:
                    if (statementFinderMemloc(t->attr.name, t->attr.scope)==-1 && statementFinderMemloc(t->attr.name, "global")== -1){ // Variavel nao esta na tabela
                        if(t->type != voidK){
                                if(t->type == arrayK)
                                    stInsert(t->attr.name, t->lineno,location++, 0, t->attr.scope, "variable", "vector arg", 2); // Nova def de variavel
                                else
                                    stInsert(t->attr.name, t->lineno,location++, 0, t->attr.scope, "variable", "integer", 1); // Nova def de variavel
                        }
                        else
                            printSemanticError(t); // Variavel declarada como void"  
                    } 
                    else
                        printSemanticError(t); //Variavel ja declarada
                    break;
        
        case functionK:
            numParam = paramCounter(t); //conta parametros
            if (statementFinderMemloc(t->attr.name, t->attr.scope) == -1 && statementFinderMemloc(t->attr.name, "global") == -1)
            {
                if (t->type == integerK)
                    stInsert(t->attr.name, t->lineno, numParam, location++, t->attr.scope, "function", "integer", 1);
                else
                    stInsert(t->attr.name, t->lineno, numParam, location++, t->attr.scope, "function", "void", 1);
            }
            else
                printSemanticError(t);// Função já foi declarada
            break;
        case callK:
            numParam = paramCounter(t);
            if (statementFinderMemloc(t->attr.name, t->attr.scope) == -1 && statementFinderMemloc(t->attr.name, "global") == -1)
                printSemanticError(t); // Chamada de função inválida
            else
                stInsert(t->attr.name, t->lineno, numParam, location++, t->attr.scope, "call", "-", 1);
        case returnK:
            break;
        default:
            break;
        }
        break;
    case expressionK:
        switch (t->kind.exp)
        {
        case idK:
            if (statementFinderMemloc(t->attr.name, t->attr.scope) == -1 && statementFinderMemloc(t->attr.name, "global") == -1)
                printSemanticError(t); // Expressão não foi declarada
            else{
                if(t->type == arrayK)
                            stInsert(t->attr.name, t->lineno, 0, 0, t->attr.scope, "variable", "vector", 1); // Insere id na tabela
                        else
                            stInsert(t->attr.name, t->lineno, 0, 0, t->attr.scope, "variable", "integer", 1); // Insere id na tabela
                    }
            break;
            
        case vectorK:
            if (statementFinderMemloc(t->attr.name, t->attr.scope) == -1 && statementFinderMemloc(t->attr.name, "global") == -1)
                printSemanticError(t); // Expressão não foi declarada
            else
                stInsert(t->attr.name, t->lineno, 0, 0, t->attr.scope, "vector", "integer", 1);
            break;
        case typeK:
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void buildSymtab(TreeNode *syntaxTree)// Cria tabela de simbolos
{
    stInsert("output",0, 0, location++, "global","função", "integer", 1);
    stInsert("input",0, 0, location++, "global","função", "integer", 1);
    traverse(syntaxTree, insertNode, nullProc);
    if (statementFinderMemloc("main", "global") == -1)
    {
        printf("ERRO SEMÂNTICO: main()\n");
        Error = TRUE;
    }
    fprintf(tabSim, "\nTabela de Símbolos:\n");
    printSymTab(tabSim);
}

static void checkNode(TreeNode *t) // Checa tipos
{
    switch (t->nodekind)
    {
    case expressionK:
        switch (t->kind.exp)
        {
        case operationK:
            break;
        default:
            break;
        }
        break;
    case statementK:
        switch (t->kind.stmt)
        {
        case ifK:
            if (t->child[0]->type == integerK && t->child[1]->type == integerK)
                printSemanticError(t->child[0]); // Expressão do IF inválida
            break;
        case assignK:
            if (t->child[0]->type == voidK || t->child[1]->type == voidK)
                printSemanticError(t->child[0]); // Atribuição de valor não inteiro
            else if (t->child[1]->kind.stmt == callK)
                if ((statementFinderType(t->child[1]->attr.name, t->child[1]->attr.scope) == voidK))
                    printSemanticError(t->child[1]); //  Atribuição de função VOID
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }
}

void typeCheck(TreeNode *syntaxTree)
{
    traverse(syntaxTree, nullProc, checkNode);
}
