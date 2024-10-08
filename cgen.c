#include "globals.h"
#include "analyze.h"
#include "cgen.h"

int nlabel;                          // Proxima label a ser criada
int globalSize;                      // Tamanho para alocacoes de variaveis globais posteriormente
QuadList head = NULL;                // Inicio da lista de quadruplas
int ntemp = 0;                       // Proximo temp a ser criado
char *tempAux, *tempAux2, *labelAux; // Auxiliares para armazenar temporarios e labels de outras chamadas
int nextDecl = 0;                    // Variavel para chamar prox declaracao
int indexFlag = 0;                   // Flag para carregar o indice em caso de STORE de um vetor

char *newLabel()
{ // Funcao que cria novo label
    char *label = (char *)malloc((nlabel_size + 3) * sizeof(char));
    sprintf(label, "L%d", nlabel);
    nlabel++;
    return label;
}

char *newTemp()
{ // Funcao que cria novo temp
    char *temp = (char *)malloc((ntemp_size + 3) * sizeof(char));
    sprintf(temp, "$t%d", ntemp);
    ntemp = (ntemp + 1) % 24; // Limite de temp $t0 - $t23
    return temp;
}

void quadInsert(char *op, char *arg1, char *arg2, char *arg3)
{ // Insere quad na lista
    Quad newQ;
    newQ.op = op;
    newQ.arg1 = arg1;
    newQ.arg2 = arg2;
    newQ.arg3 = arg3;
    QuadList newQL = (QuadList)malloc(sizeof(struct QuadListRec));
    newQL->quad = newQ;
    newQL->next = NULL;
    if (head == NULL)
        head = newQL;
    else
    {
        QuadList q = head;
        while (q->next != NULL)
            q = q->next;
        q->next = newQL;
    }
}

void genStatement(TreeNode *tree)
{
    char *arg1, *arg2, *arg3, *aux1, *aux2;

    switch (tree->kind.stmt)
    {
    case ifK:
        cGen(tree->child[0]);
        arg1 = tempAux;
        labelAux = newLabel();
        arg2 = labelAux;
        aux1 = arg2;
        arg3 = "-";
        quadInsert("IFF", arg1, arg2, arg3);

        // IF true
        cGen(tree->child[1]);
        if (tree->child[2] != NULL)
        { // ELSE not NULL
            labelAux = newLabel();
            arg1 = labelAux;
            aux2 = arg1;
            arg2 = "-";
            arg3 = "-";
            quadInsert("GOTO", arg1, arg2, arg3);
        }

        // IF false
        if (tree->child[2] != NULL)
        {
            arg1 = aux1;
            arg2 = "-";
            arg3 = "-";
            quadInsert("LABEL", arg1, arg2, arg3);
            cGen(tree->child[2]);
        }

        // Saida IF
        if (tree->child[2] != NULL)
            arg1 = aux2;
        else
            arg1 = aux1;
        arg2 = "-";
        arg3 = "-";
        quadInsert("LABEL", arg1, arg2, arg3);
        break;

    case whileK:
        // LABEL para repeticoes
        labelAux = newLabel();
        arg1 = labelAux;
        aux1 = arg1;
        arg2 = "-";
        arg3 = "-";
        quadInsert("LABEL", arg1, arg2, arg3);

        // Checagem da condicao do WHILE
        cGen(tree->child[0]);
        arg1 = tempAux;
        labelAux = newLabel();
        arg2 = labelAux;
        aux2 = arg2;
        arg3 = "-";
        quadInsert("IFF", arg1, arg2, arg3);

        // Declaracoes do WHILE
        cGen(tree->child[1]);
        arg1 = aux1;
        arg2 = "-";
        arg3 = "-";
        quadInsert("GOTO", arg1, arg2, arg3);

        // Saida do WHILE
        arg1 = aux2;
        arg2 = "-";
        arg3 = "-";
        quadInsert("LABEL", arg1, arg2, arg3);
        break;

    case assignK:
        if (tree->child[0]->kind.exp == vectorK) // Caso a variavel em que deve ser armazenada seja um vetor
            indexFlag = 1;                       // Deve-se carregar o indice calculado na funcao genExpression
        cGen(tree->child[0]);
        arg1 = tempAux;
        cGen(tree->child[1]);
        arg2 = tempAux;
        arg3 = "-";
        quadInsert("ASSIGN", arg1, arg2, arg3);

        if (indexFlag > 1)
        {                    // Caso a variavel em que deve ser armazenada seja um vetor
            arg3 = tempAux2; // Carrega-se o temp usado como indice
            indexFlag--;     // Reseta-se a flag para uso posterior caso necessario
        }
        aux1 = arg1;
        arg1 = tree->child[0]->attr.name;
        arg2 = aux1;
        quadInsert("STORE", arg1, arg2, arg3);
        break;

    case variableK:
        arg1 = tree->attr.name;
        arg2 = tree->attr.scope;
        if (tree->type == arrayK)
        {
            arg3 = (char *)malloc(sizeof(char *));
            sprintf(arg3, "%d", tree->attr.len);
            quadInsert("ALLOC", arg1, arg2, arg3);
            if (strcmp(arg2, "global") == 0)
                globalSize += tree->attr.len;
        }
        else
        {
            arg3 = "-";
            quadInsert("ALLOC", arg1, arg2, arg3);
            if (strcmp(arg2, "global") == 0)
                globalSize += 1;
        }
        break;

    case paramK:
        if (tree->type == integerK)
            arg1 = "int";
        else
            arg1 = "int[]";
        arg2 = tree->attr.name;
        arg3 = tree->attr.scope;
        quadInsert("ARG", arg1, arg2, arg3);

        break;

    case functionK:
        if (tree->type == integerK)
            arg1 = "int";
        else
            arg1 = "void";
        arg2 = tree->attr.name;
        arg3 = "-";
        quadInsert("FUN", arg1, arg2, arg3);
        cGen(tree->child[0]);
        cGen(tree->child[1]);
        arg1 = arg2;
        arg2 = "-";
        arg3 = "-";
        quadInsert("END", arg1, arg2, arg3);
        break;

    case callK:
        genCallParams(tree->child[0]);
        tempAux = newTemp();
        arg1 = tempAux;
        arg2 = tree->attr.name;
        arg3 = (char *)malloc(sizeof(char *));
        sprintf(arg3, "%d", paramCounter(tree));
        quadInsert("CALL", arg1, arg2, arg3);
        break;

    case returnK:
        cGen(tree->child[0]);
        arg1 = tempAux;
        arg2 = "-";
        arg3 = "-";
        quadInsert("RET", arg1, arg2, arg3);
        break;

    default:
        break;
    }
}

void genExpression(TreeNode *tree)
{
    char *arg1, *arg2, *arg3;
    switch (tree->kind.exp)
    {
    case operationK:
        cGen(tree->child[0]);
        arg2 = tempAux;
        cGen(tree->child[1]);
        arg3 = tempAux;
        tempAux = newTemp();
        arg1 = tempAux;

        switch (tree->attr.op)
        {
        case SUM:
            quadInsert("ADD", arg1, arg2, arg3);
            break;
        case SUBTRACT:
            quadInsert("SUB", arg1, arg2, arg3);
            break;
        case MULTIPLY:
            quadInsert("MUL", arg1, arg2, arg3);
            break;
        case DIVIDE:
            quadInsert("DIV", arg1, arg2, arg3);
            break;
        case LESS_THAN:
            quadInsert("LT", arg1, arg2, arg3);
            break;
        case LESS_THAN_EQUAL:
            quadInsert("LET", arg1, arg2, arg3);
            break;
        case GREATER_THAN:
            quadInsert("GT", arg1, arg2, arg3);
            break;
        case GREATER_THAN_EQUAL:
            quadInsert("GET", arg1, arg2, arg3);
            break;
        case EQ:
            quadInsert("EQ", arg1, arg2, arg3);
            break;
        case NOT_EQUAL:
            quadInsert("NEQ", arg1, arg2, arg3);
            break;
        default:
            break;
        }
        break;

    case constantK:
        tempAux = newTemp();
        arg1 = tempAux;
        arg2 = (char *)malloc(sizeof(char *));
        sprintf(arg2, "%d", tree->attr.val);
        arg3 = "-";
        quadInsert("LOAD", arg1, arg2, arg3);
        break;

    case idK:
        tempAux = newTemp();
        arg1 = tempAux;
        arg2 = tree->attr.name;
        arg3 = "-";
        quadInsert("LOAD", arg1, arg2, arg3);
        break;

    case vectorK:
        cGen(tree->child[0]);
        arg3 = tempAux;
        if (indexFlag == 1)
        {
            tempAux2 = arg3;
            indexFlag++;
        }
        tempAux = newTemp();
        arg1 = tempAux;
        arg2 = tree->attr.name;
        quadInsert("LOAD", arg1, arg2, arg3);
        break;

    case typeK:
        cGen(tree->child[0]);
        break;

    default:
        break;
    }
}

void genCallParams(TreeNode *tree)
{
    TreeNode *treeAux;
    char *arg1, *arg2, *arg3;
    treeAux = tree;
    while (treeAux != NULL)
    {
        switch (treeAux->nodekind)
        {
        case statementK:
            genStatement(treeAux);
            break;
        case expressionK:
            genExpression(treeAux);
            break;
        default:
            break;
        }
        arg1 = tempAux;
        arg2 = "-";
        arg3 = "-";
        quadInsert("PARAM", arg1, arg2, arg3);
        treeAux = treeAux->sibling;
    }
}

void cGen(TreeNode *tree)
{
    if (tree != NULL)
    {
        nextDecl++; // nextDecl = variavel para chamar prox declaracao
        switch (tree->nodekind)
        {
        case statementK:
            genStatement(tree);
            break;
        case expressionK:
            genExpression(tree);
            break;
        default:
            break;
        }
        nextDecl--;
        if (nextDecl < 0)
            cGen(tree->child[1]);
        else
            cGen(tree->sibling);
    }
}

void printCode(FILE *codefile)
{
    QuadList q = head;
    while (q != NULL)
    {
        fprintf(codefile, "(%s, ", q->quad.op);
        fprintf(codefile, "%s, ", q->quad.arg1);
        fprintf(codefile, "%s, ", q->quad.arg2);
        fprintf(codefile, "%s)\n", q->quad.arg3);
        q = q->next;
    }
}

QuadList codeGen(TreeNode *syntaxTree)
{
    nlabel = 0;
    globalSize = 0;
    cGen(syntaxTree);
    quadInsert("HLT", "-", "-", "-");
    FILE *codefile = fopen("output/codigoIntermediario.txt", "w+");
    printCode(codefile);
    fclose(codefile);
    return head;
}
