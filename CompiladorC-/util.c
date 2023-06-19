#include "globals.h"
#include "util.h"

void printToken(TokenType token, const char *tokenString)
{
  switch (token)
  {
  case IF:
    fprintf(arvSin, "Reserved Word: %s\n", tokenString);
    break;
  case ELSE:
    fprintf(arvSin, "Reserved Word: %s\n", tokenString);
    break;
  case INT:
    fprintf(arvSin, "Reserved Word: %s\n", tokenString);
    break;
  case RETURN:
    fprintf(arvSin, "Reserved Word: %s\n", tokenString);
    break;
  case VOID:
    fprintf(arvSin, "Reserved Word: %s\n", tokenString);
    break;
  case WHILE:
    fprintf(arvSin, "Reserved Word: %s\n", tokenString);
    break;
  case ASSIGN:
    fprintf(arvSin, "Symbol: =\n");
    break;
  case LESS_THAN:
    fprintf(arvSin, "<\n");
    break;
  case EQ:
    fprintf(arvSin, "==\n");
    break;
  case GREATER_THAN:
    fprintf(arvSin, ">\n");
    break;
  case LESS_THAN_EQUAL:
    fprintf(arvSin, "<=\n");
    break;
  case GREATER_THAN_EQUAL:
    fprintf(arvSin, ">=\n");
    break;
  case NOT_EQUAL:
    fprintf(arvSin, "!=\n");
    break;
  case OPEN_BRACKET:
    fprintf(arvSin, "[\n");
    break;
  case CLOSE_BRACKET:
    fprintf(arvSin, "]\n");
    break;
  case OPEN_KEYS:
    fprintf(arvSin, "{\n");
    break;
  case CLOSE_KEYS:
    fprintf(arvSin, "}\n");
    break;
  case OPEN_PARENTHESES:
    fprintf(arvSin, "(\n");
    break;
  case CLOSE_PARENTHESES:
    fprintf(arvSin, ")\n");
    break;
  case SEMICOLON:
    fprintf(arvSin, ";\n");
    break;
  case COMMA:
    fprintf(arvSin, ",\n");
    break;
  case SUM:
    fprintf(arvSin, "+\n");
    break;
  case SUBTRACT:
    fprintf(arvSin, "-\n");
    break;
  case MULTIPLY:
    fprintf(arvSin, "*\n");
    break;
  case DIVIDE:
    fprintf(arvSin, "/\n");
    break;
  case ENDFILE:
    fprintf(arvSin, "EOF\n");
    break;
  case NUM:
    fprintf(arvSin, "Number: %s\n", tokenString);
    break;
  case ID:
    fprintf(arvSin, "ID: %s\n", tokenString);
    break;
  case ERROR:
    fprintf(arvSin, "ERROR: %s\n", tokenString);
    break;
  default:
    fprintf(arvSin, "Unknown token: %d\n", token);
  }
}

void printTokenForError(TokenType token, const char *tokenString)
{
  switch (token)
  {
  case IF:
    fprintf(arvSin, "%s", tokenString);
    break;
  case ELSE:
    fprintf(arvSin, "%s", tokenString);
    break;
  case INT:
    fprintf(arvSin, "%s", tokenString);
    break;
  case RETURN:
    fprintf(arvSin, "%s", tokenString);
    break;
  case VOID:
    fprintf(arvSin, "%s", tokenString);
    break;
  case WHILE:
    fprintf(arvSin, "%s", tokenString);
    break;
  case ASSIGN:
    fprintf(arvSin, "=");
    break;
  case LESS_THAN:
    fprintf(arvSin, "<");
    break;
  case EQ:
    fprintf(arvSin, "==");
    break;
  case GREATER_THAN:
    fprintf(arvSin, ">");
    break;
  case LESS_THAN_EQUAL:
    fprintf(arvSin, "<=");
    break;
  case GREATER_THAN_EQUAL:
    fprintf(arvSin, ">=");
    break;
  case NOT_EQUAL:
    fprintf(arvSin, "!=");
    break;
  case OPEN_BRACKET:
    fprintf(arvSin, "[");
    break;
  case CLOSE_BRACKET:
    fprintf(arvSin, "]");
    break;
  case OPEN_KEYS:
    fprintf(arvSin, "{");
    break;
  case CLOSE_KEYS:
    fprintf(arvSin, "}");
    break;
  case OPEN_PARENTHESES:
    fprintf(arvSin, "(");
    break;
  case CLOSE_PARENTHESES:
    fprintf(arvSin, ")");
    break;
  case SEMICOLON:
    fprintf(arvSin, ";");
    break;
  case COMMA:
    fprintf(arvSin, ",");
    break;
  case SUM:
    fprintf(arvSin, "+");
    break;
  case SUBTRACT:
    fprintf(arvSin, "-");
    break;
  case MULTIPLY:
    fprintf(arvSin, "*");
    break;
  case DIVIDE:
    fprintf(arvSin, "/");
    break;
  case ENDFILE:
    fprintf(arvSin, "EOF");
    break;
  case NUM:
    fprintf(arvSin, "%s", tokenString);
    break;
  case ID:
    fprintf(arvSin, "%s", tokenString);
    break;
  case ERROR:
    fprintf(arvSin, "%s", tokenString);
    break;
  default:
    fprintf(arvSin, "%d", token);
  }
}

void aggScope(TreeNode *t, char *scope)
{
  int i;
  while (t != NULL)
  {
    for (i = 0; i < MAXCHILDREN; ++i)
    {
      t->attr.scope = scope;
      aggScope(t->child[i], scope);
    }
    t = t->sibling;
  }
}

TreeNode *newStmtNode(StatementKind kind)
{
  TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
  int i;
  if (t == NULL)
    fprintf(arvSin, "Out of memory error at line %d\n", lineno);
  else
  {
    for (i = 0; i < MAXCHILDREN; i++)
      t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = statementK;
    t->kind.stmt = kind;
    t->lineno = lineno;
    t->attr.scope = "global";
  }
  return t;
}

TreeNode *newExpNode(ExpressionIdentifier kind)
{
  TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
  int i;
  if (t == NULL)
    fprintf(arvSin, "Out of memory error at line %d\n", lineno);
  else
  {
    for (i = 0; i < MAXCHILDREN; i++)
      t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = expressionK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = VOID;
    t->attr.scope = "global";
  }
  return t;
}

char *copyString(char *s)
{
  int n;
  char *t;
  if (s == NULL)
    return NULL;
  n = strlen(s) + 1;
  t = malloc(n);
  if (t == NULL)
    fprintf(arvSin, "Out of memory error at line %d\n", lineno);
  else
    strcpy(t, s);
  return t;
}

static int indentno = 0;
static int primeiraLinha = 0;

#define INDENT indentno += 2
#define UNINDENT indentno -= 2

static void printSpaces(void)
{
  int i;
  for (i = 0; i < indentno; i++)
    fprintf(arvSin, " ");
}

void printTree(TreeNode *tree)
{
  int i;
  if (primeiraLinha == 0)
		primeiraLinha = 1;
	else
  		INDENT;
  while (tree != NULL)
  {
    printSpaces();
    if (tree->nodekind == statementK)
    {
      switch (tree->kind.stmt)
      {
      case ifK:
        fprintf(arvSin, "If\n");
        break;
      case assignK:
        fprintf(arvSin, "Assign\n");
        break;
      case whileK:
        fprintf(arvSin, "While\n");
        break;
      case variableK:
        fprintf(arvSin, "Variable %s\n", tree->attr.name);
        break;
      case functionK:
        fprintf(arvSin, "Function %s\n", tree->attr.name);
        break;
      case callK:
        fprintf(arvSin, "Call to Function %s \n", tree->attr.name);
        break;
      case returnK:
        fprintf(arvSin, "Return\n");
        break;

      default:
        fprintf(arvSin, "Unknown ExpNode kind\n");
        break;
      }
    }
    else if (tree->nodekind == expressionK)
    {
      switch (tree->kind.exp)
      {
      case operationK:
        fprintf(arvSin, "Operation: ");
        printToken(tree->attr.op, "\0");
        break;
      case constantK:
        fprintf(arvSin, "Constant: %d\n", tree->attr.val);
        break;
      case idK:
        fprintf(arvSin, "Id: %s\n", tree->attr.name);
        break;
      case vectorK:
        fprintf(arvSin, "Vector: %s\n", tree->attr.name);
        break;
      case vectorIdK:
        fprintf(arvSin, "Index [%d]\n", tree->attr.val);
        break;
      case typeK:
        fprintf(arvSin, "Type %s\n", tree->attr.name);
        break;

      default:
        fprintf(arvSin, "Unknown ExpNode kind\n");
        break;
      }
    }
    else
      fprintf(arvSin, "Unknown node kind\n");
    for (i = 0; i < MAXCHILDREN; i++)
      printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}
