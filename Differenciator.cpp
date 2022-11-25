#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>

#include "Tree.h"
#include "Differenciator.h"



//-------------------------------------------------------

const char* formula_string;

Node* GetG(const char** str)
{
    formula_string = *str;
    Node* new_Node = GetE(str);
    assert(**str == '\0');
    return new_Node;
}

Node* GetE(const char** str)
{
    //printf("GetE!\n");
    Node* left_node = GetT(str);

    while (**str == '+' || **str == '-')
    {
        char op = **str;
        (*str)++;

        Node* right_node = GetT(str);
        Node* op_node = NULL;

        if (op == '+')  //val += val2;
        {
            op_node = NodeCtor(OPERATOR, PLUS, 0, left_node, right_node);
        }
        else
        {
            op_node = NodeCtor(OPERATOR, MINUS, 0, left_node, right_node);
        }

        left_node->parent = op_node;
        right_node->parent = op_node;

        left_node = op_node;
    }
    
    return left_node;
}

Node* GetT(const char** str)
{
    //printf("GetT!\n");
    Node* left_node = GetP(str);

    while (**str == '*' || **str == '/')
    {
        char op = **str;
        (*str)++;

        Node* right_node = GetP(str);
        Node* op_node = NULL;

        if (op == '*')  //val += val2;
        {
            op_node = NodeCtor(OPERATOR, MUL, 0, left_node, right_node);
        }
        else
        {
            op_node = NodeCtor(OPERATOR, DIV, 0, left_node, right_node);
        }

        left_node->parent = op_node;
        right_node->parent = op_node;

        left_node = op_node;
    }
    
    return left_node;
}

Node* GetP(const char** str)
{
    //printf("GetP!\n");
    Node* new_node = NULL;

    if (**str == '(')
    {
        (*str)++;
        new_node = GetE(str);
        assert(**str == ')');
        (*str)++;
    }

    else if (**str == 's' || **str == 'c' || **str == 't' || **str == 'e')
    {
        int op = Get_Operator(str);
        switch (op)
        {
        case SIN:
            new_node = NodeCtor(OPERATOR, SIN);
            break;

        case COS:
            new_node = NodeCtor(OPERATOR, COS);
            break;

        case TG:
            new_node = NodeCtor(OPERATOR, TG);
            break;

        case CTG:
            new_node = NodeCtor(OPERATOR, CTG);
            break;

        case EXP:
            new_node = NodeCtor(OPERATOR, EXP);
            break;

        case UNKNOWN_OPERATOR:
            Syntax_Error(str);
            break;
        
        default:
            printf("Как ты, блин, здесь оказался?\n");
            break;
        }

        Node* argument_node = GetP(str);

        argument_node->parent = new_node;
        new_node->right = argument_node;
    }

    else
    {
        new_node = GetN(str);
    }

    return new_node;
}

Node* GetN(const char** str)
{
    //printf("GetN!\n");
    double val = 0;
    const char* str_old = *str;
    while ('0' <= **str && **str <= '9')
    {
        val = val*10 + **str - '0';
        (*str)++;
    }
    if (**str == '.')
    {
        (*str)++;
        int i = 1;
        while ('0' <= **str && **str <= '9')
        {
            double add = (double)(**str - '0');
            for (int j = 0; j < i; j++) add /= 10;
            val = val + add; //((double)(**str - '0')/(10 * i));
            printf("%lf\n", val);
            (*str)++;
            i++;
        }
    }
    
    //assert(*str != str_old);

    Node* new_node = NULL;

    if (*str == str_old)
    {
        if (**str == 'x')
        {
            new_node = NodeCtor(VARIABLE, VARIABLE);
            (*str)++;
        }
        else Syntax_Error(str);
    }
    else new_node = NodeCtor(NUMBER, 0, val);

    return new_node;
}

void Syntax_Error(const char** str)
{
    printf("ERROR! You have a syntax error in string:\n");
    int lenth = strlen(formula_string);

    for (int i = 0; i < *str - formula_string; i++) printf("%3d", i);
    printf("\033[31m%3d\033[0m", *str - formula_string);
    for (int i = *str - formula_string + 1; i < lenth; i++) printf("%3d", i);
    printf("\n");

    for (int i = 0; i < *str - formula_string; i++) printf("%3c", *(formula_string + i));
    printf("\033[31m%3c\033[0m", *(formula_string + (*str - formula_string)));
    for (int i = (*str - formula_string) + 1; i < lenth; i++) printf("%3c", *(formula_string + i));
    printf("\n");

    exit(666);
}

Operators Get_Operator(const char** str)
{
    const char* string = *str;

    char read[4] = {};

    read[0] = string[0];
    read[1] = string[1];

    if (strcmp(read, "tg") == 0)
    {
        *(str) += 2;
        return TG;
    }
    else if (strcmp(read, "e^") == 0)
    {
        *(str) += 2;
        return EXP;
    }

    read[2] = string[2];

    if (strcmp(read, "sin") == 0)
    {
        *(str) += 3;
        return SIN;
    }

    else if (strcmp(read, "cos") == 0)
    {
        *(str) += 3;
        return COS;
    }

    else if (strcmp(read, "ctg") == 0)
    {
        *(str) += 3;
        return CTG;
    }

    else if (strcmp(read, "exp") == 0)
    {
        *(str) += 3;
        return EXP;
    }

    else return UNKNOWN_OPERATOR;
}

//-------------------------------------------------------



//-------------------------------------------------------

int File_Size(FILE* file)
{
    struct stat buff = {};
    fstat(fileno(file), &buff);
    int size = buff.st_size;
    return size;
}


char* Get_String(FILE* stream, int size)
{
    char* string = (char*)calloc(size, sizeof(char) + 1);

    char c = fgetc(stream);
    int i = 0;
    while (c != '\n' && c != '\0' && c != EOF)
    {
        if (c != ' ')
        {
            *(string + i) = c;
            i++;
        }
        c = fgetc(stream);
    }
    
    return(string);
}

void Differenciator_TreeCtor(Tree* tree)
{
    FILE* inp_file = fopen("inp.txt", "r");

    const char* string = Get_String(inp_file, File_Size(inp_file));
    printf("%s\n", string);

    TreeCtor(tree);
    tree->root = GetG(&string);
    Tree_Dump(tree->root);
}

int main()
{
    Tree tree = {};
    Differenciator_TreeCtor(&tree);
}