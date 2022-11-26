#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <locale.h>

#include "Tree.h"
#include "Differenciator.h"



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

Node* Copy_SubTree(Node* node)
{
    printf("gav\n");
    Node* new_node = NodeCtor(node->type, node->value, node->num_value, node->left, node->right, node->parent);
    if (new_node->left != NULL) new_node->left = Copy_SubTree(new_node->left);
    if (new_node->right != NULL) new_node->right = Copy_SubTree(new_node->right);
    if (new_node->left != NULL) new_node->left->parent = new_node;
    if (new_node->right != NULL) new_node->right->parent = new_node;
    return new_node;
}

Node* Differenciate_Node(Node* node)
{
    extern int image_num;
    printf("I'm in differenciator, now dump%d\n", image_num);
    Tree_Dump(node);
    Node* new_node = NULL;

    Node* node_chislitel_1 = NULL;
    Node* node_chislitel_2 = NULL;
    Node* chislitel        = NULL;
    Node* znamenatel       = NULL;
    Node* part_1           = NULL;
    Node* part_2           = NULL;
    Node* sin_node         = NULL;
    Node* sin_node_1       = NULL;
    Node* sin_node_2       = NULL;
    Node* cos_node         = NULL;
    Node* cos_node_1       = NULL;
    Node* cos_node_2       = NULL;
    Node* left_part        = NULL;
    Node* square           = NULL;
    Node* out_function     = NULL;
    Node* left             = NULL;

    switch (node->type)
    {
    case NUMBER:
        new_node = NodeCtor(NUMBER, 0, 0);
        break;

    case VARIABLE:
        new_node = NodeCtor(NUMBER, 0, 1);
        break;

    case OPERATOR:
        switch (node->value)
        {
        case PLUS:
            new_node = NodeCtor(OPERATOR, PLUS, 0, Differenciate_Node(node->left), Differenciate_Node(node->right));
            new_node->left->parent = new_node;
            new_node->right->parent = new_node;
            break;
        
        case MINUS:
            new_node = NodeCtor(OPERATOR, MINUS, 0, Differenciate_Node(node->left), Differenciate_Node(node->right));
            new_node->left->parent = new_node;
            new_node->right->parent = new_node;
            break;

        case DIV:
            node_chislitel_1 = NodeCtor(OPERATOR, MUL, 0, Differenciate_Node(node->left), Copy_SubTree(node->right));
            node_chislitel_2 = NodeCtor(OPERATOR, MUL, 0, Copy_SubTree(node->left), Differenciate_Node(node->right));
            chislitel        = NodeCtor(OPERATOR, MINUS, 0, node_chislitel_1, node_chislitel_2);
            znamenatel       = NodeCtor(OPERATOR, MUL, 0, Copy_SubTree(node->right), Copy_SubTree(node->right));
            new_node         = NodeCtor(OPERATOR, DIV, 0, chislitel, znamenatel);
            node_chislitel_1->parent = chislitel;
            node_chislitel_2->parent = chislitel;
            znamenatel->left->parent = znamenatel;
            znamenatel->right->parent = znamenatel;
            chislitel->parent - new_node;
            znamenatel->parent = new_node;
            break;

        case MUL:
            printf("meow mull\n");
            part_1           = NodeCtor(OPERATOR, MUL, 0, Differenciate_Node(node->left), Copy_SubTree(node->right), new_node);
            printf("mewo mull1\n");
            part_2           = NodeCtor(OPERATOR, MUL, 0, Copy_SubTree(node->left), Differenciate_Node(node->right), new_node);
            printf("mewo mull2\n");
            new_node         = NodeCtor(OPERATOR, PLUS, 0, part_1, part_2);
            printf("mewo mull3\n");
            part_1->parent = new_node;
            part_2->parent = new_node;
            break;

        case SIN:
            cos_node         = NodeCtor(OPERATOR, COS, 0, NULL, Copy_SubTree(node->right));
            new_node         = NodeCtor(OPERATOR, MUL, 0, cos_node, Differenciate_Node(node->right));
            cos_node->right->parent = cos_node;
            cos_node->parent = new_node;
            new_node->right->parent = new_node;
            break;

        case COS:
            sin_node         = NodeCtor(OPERATOR, SIN, 0, NULL, Copy_SubTree(node->right));
            left_part        = NodeCtor(OPERATOR, MUL, 0, NodeCtor(NUMBER, NUMBER, -1), sin_node);
            new_node         = NodeCtor(OPERATOR, MUL, 0, left_part, Differenciate_Node(node->right));
            left_part->left->parent = left_part;            
            new_node->right->parent = new_node;
            break;

        case TG:
            cos_node_1       = NodeCtor(OPERATOR, COS, 0, NULL, Copy_SubTree(node->right));
            cos_node_2       = Copy_SubTree(cos_node_1);
            square           = NodeCtor(OPERATOR, MUL, 0, cos_node_1, cos_node_2);
            out_function     = NodeCtor(OPERATOR, DIV, 0, NodeCtor(NUMBER, NUMBER, 1), square);
            new_node         = NodeCtor(OPERATOR, MUL, 0, out_function, Differenciate_Node(node->right));
            cos_node_1->parent = square;
            cos_node_2->parent = square;
            cos_node_1->left->parent = cos_node_1;
            cos_node_1->right->parent = cos_node_1;
            cos_node_2->left->parent = cos_node_2;
            cos_node_2->right->parent = cos_node_2;
            square->parent = out_function;
            out_function->left->parent = out_function;
            new_node->left->parent = new_node;
            new_node->right->parent = new_node;
            break;

        case CTG:
            sin_node_1       = NodeCtor(OPERATOR, SIN, 0, NULL, Copy_SubTree(node->right));
            sin_node_2       = Copy_SubTree(sin_node_1);
            square           = NodeCtor(OPERATOR, MUL, 0, sin_node_1, sin_node_2);
            left             = NodeCtor(OPERATOR, MUL, 0, NodeCtor(NUMBER, NUMBER, -1), square);
            out_function     = NodeCtor(OPERATOR, DIV, 0, NodeCtor(NUMBER, NUMBER, 1), left);
            new_node         = NodeCtor(OPERATOR, MUL, 0, out_function, Differenciate_Node(node->right));
            sin_node_1->parent = square;
            sin_node_2->parent = square;
            sin_node_1->left->parent = sin_node_1;
            sin_node_1->right->parent = sin_node_1;
            sin_node_2->right->parent = sin_node_2;
            sin_node_2->left->parent = sin_node_2;
            left->left->parent = left;
            left->right->parent = left;
            out_function->left->parent = out_function;
            out_function->right->parent = out_function;
            new_node->left->parent = new_node;
            new_node->right->parent = new_node;
            break;
        
        case EXP:
            new_node               = NodeCtor(OPERATOR, MUL, 0, Copy_SubTree(node->right), Differenciate_Node(node->right));
            new_node->left->parent = new_node;
            new_node->right->parent = new_node;
            break;

        
        default:
            printf("I am maloletniy debil\n");
            break;
        }

    break;
    
    default:
        printf("You are maloletniy debil\n");
        break;
    }

    return new_node;
}

Tree Differenciate_Tree(Tree* tree)
{
    Tree proizvodnaya = {};
    proizvodnaya.root = Differenciate_Node(tree->root);
    Tree_Dump(proizvodnaya.root);
}

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
    setlocale(LC_ALL, "Rus");
    Tree tree = {};
    Differenciator_TreeCtor(&tree);
    Differenciate_Tree(&tree);
}