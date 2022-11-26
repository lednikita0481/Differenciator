#include <stdio.h>
#include <stdlib.h>
//#include "Tree.h"


Node* GetG(const char** str);
Node* GetE(const char** str);
Node* GetT(const char** str);
Node* GetP(const char** str);
Node* GetN(const char** str);
void Syntax_Error(const char** str);
Operators Get_Operator(const char** str);
//--------------------------------------------------------
Node* Copy_SubTree(Node* node);
Node* Differenciate_Node(Node* node);
//--------------------------------------------------------

int File_Size(FILE* file);
char* Get_String(FILE* stream, int size);
void Differenciator_TreeCtor(Tree* tree);