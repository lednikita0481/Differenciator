#include <stdio.h>

enum Node_Types
{
    NUMBER,
    VARIABLE,
    OPERATOR,
};

enum Operators
{
    PLUS,
    MINUS,
    MUL,
    DIV,
    SIN,
    COS,
    TG,
    CTG,
    EXP,
    POW,
    UNKNOWN_OPERATOR
};

union Node_Value
{
    int int_value;
    double double_value;
};

struct Node
{
    Node_Types type;

    int value;
    double num_value;

    Node* parent;
    Node* left;
    Node* right;

    int cell_num;
};

struct Tree
{
    Node* root;
    size_t size;
};



#define DUMP(text...)\
    fprintf(dump_file, text);    

void TreeCtor(Tree* tree);
Node* NodeCtor(Node_Types type, int value = VARIABLE, double num_value = NULL, Node* left = NULL, Node* right = NULL, Node* parent = NULL);
const char* Type_Name(Node_Types type);
const char* Value_Name(Node* node);
void Node_Destructor(Node* node);
void Nodes_Destructor(Node* node);
void Recursive_Node_Dump(Node* node, FILE* dump_file);
void Tree_Dump(Node* root_node, char* func);
    
