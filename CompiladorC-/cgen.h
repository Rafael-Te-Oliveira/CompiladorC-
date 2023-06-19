#define nlabel_size 3
#define ntemp_size 3

typedef struct{
    char * op, * arg1, * arg2, * arg3;   
} Quad;

typedef struct QuadListRec{
    Quad quad;
    struct QuadListRec * next; 
} * QuadList;

QuadList codeGen(TreeNode * syntaxTree);

void cGen(TreeNode * tree);

void genCallParams(TreeNode * tree);

