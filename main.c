#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
#include "analyze.h"
#include "cgen.h"
#include "assembly.h"
#include "binary.h"

/* Variaveis globais */
int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;
FILE *arvSin;
FILE *tabSim;

int Error = FALSE;

int main(int argc, char *argv[])
{
    TreeNode *syntaxTree;
    char pgm[120];
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    strcpy(pgm, argv[1]);
    if (strchr(pgm, '.') == NULL)
        strcat(pgm, ".cminus");
    source = fopen(pgm, "r");
    if (source == NULL)
    {
        fprintf(stderr, "Arquivo %s não encontrado\n", pgm);
        exit(1);
    }

    listing = stdout;
    arvSin = fopen("output/arvoreSintatica.output", "w+");
    tabSim = fopen("output/tabelaSimbolos.output", "w+");

    printf("\nINICIANDO COMPILAÇÃO: %s\n", pgm);
    syntaxTree = parse();
    fprintf(listing, "\nGerando arvore sintática...\n");
    printTree(syntaxTree);

    if (!Error)
    {
        QuadList quadList;
        InstructionList assemblyList;

        fprintf(listing, "\nGerando tabela de símbolos...\n");
        buildSymtab(syntaxTree);
        typeCheck(syntaxTree);

        printf("\nGerando Codigo Intermediario...\n");
        quadList = codeGen(syntaxTree);

        printf("\nGerando Codigo Assembly...\n");
        assemblyList = assemblyGen(quadList);

        printf("\nGerando Codigo Binário...\n");
        binaryGen(assemblyList);

        printf("\nConcluído\n");
    }

    fclose(source);
    fclose(tabSim);
    fclose(arvSin);
    return 0;
}
