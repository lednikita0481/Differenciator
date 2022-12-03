#include <stdio.h>
#include <stdlib.h>
//#include "Tree.h"


Node* GetG(const char** str);
Node* GetE(const char** str);
Node* GetT(const char** str);
Node* GetW(const char** str);
Node* GetP(const char** str);
Node* GetN(const char** str);
void Syntax_Error(const char** str);
Operators Get_Operator(const char** str);
//--------------------------------------------------------
Node* Copy_SubTree(Node* node);
Node* Differenciate_Node(Node* node);
Tree Differenciate_Tree(Tree* tree);
//--------------------------------------------------------
void Replace_Nodes(Node* old_node, Node* new_node);
bool EASY_PEASY(Node** NODE1, int rec_depth = 0);
void Change_Vars(Node* node, double x);
double Count_Value(Tree* tree, double x);
//--------------------------------------------------------
int File_Size(FILE* file);
char* Get_String(FILE* stream, int size);
void Differenciator_TreeCtor(Tree* tree);
//--------------------------------------------------------