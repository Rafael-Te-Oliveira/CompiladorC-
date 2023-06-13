#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
#include "analyze.h"
#include "cgen.h"


/* Variaveis globais */
int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;

/* Flags */
int EchoSource = FALSE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = FALSE;

int Error = FALSE;

main(int argc, char *argv[])
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
        strcat(pgm, ".cms");
    source = fopen(pgm, "r");
    if (source == NULL)
    {
        fprintf(stderr, "Arquivo %s não encontrado\n", pgm);
        exit(1);
    }
    listing = stdout;
    fprintf(listing, "\nINICIANDO COMPILAÇÃO: %s\n", pgm);
#if NO_PARSE
    while (getToken() != ENDFILE);
#else
    syntaxTree = parse();
    if (TraceParse)
    {
        fprintf(listing, "\nÁrvore sintática:\n");
        printTree(syntaxTree);
    }
#if !NO_ANALYZE
    if (!Error)
    {
        QuadList quadList;
        
        fprintf(listing, "\nTabela de símbolos:\n");
        buildSymtab(syntaxTree);

        fprintf(listing, "\nInício da verificação semântica\n");
        typeCheck(syntaxTree);
        fprintf(listing, "\nVerificação semântica concluída\n");

        printf("\nGeração de Codigo Intermediario.\n");
        quadList = codeGen(syntaxTree);
        printf("\nCodigo Intermediario gerado.\n");
    }

#endif
#endif
    fclose(source);
    return 0;
}
