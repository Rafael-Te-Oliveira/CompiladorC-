#ifndef _SYMTAB_H_
#define _SYMTAB_H_

/* Procedure stInsert inserts line numbers and
 * memory locations into the symbol table
 * memloc = memory location is inserted only the
 * first time, otherwise ignored
 */
void stInsert( char * name, int lineno,int nParam, int memloc, char* scope, char* typeID, char* typeData, int size);

/* Function statementFinder returns the memory 
 * location of a variable or -1 if not found
 */
int statementFinder (char * name, char* scope );

int statementFinderMemloc (char* name, char* scope);

char* statementFinderType(char* name, char* scope);

int statementFinderNparam (char *name, char *scope);

int statementFinderSize(char *name, char *scope);

int statementFinderInstLine(char * name);

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing);

void insertMemoryData(char * name, char * scope, int location, int * instLine);

void printMemInfo();

#endif
