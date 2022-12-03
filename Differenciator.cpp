#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <locale.h>
#include <math.h>

#include "Tree.h"
#include "Differenciator.h"
#include "DSL.h"
#include "Tex.h"
//#include "TXLib.h"



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
    Node* left_node = GetW(str);

    while (**str == '*' || **str == '/')
    {
        char op = **str;
        (*str)++;

        Node* right_node = GetW(str);
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

Node* GetW(const char** str)
{
    Node* left_node = GetP(str);

    if (**str == '^')
    {
        (*str)++;
        Node* right_node = GetP(str);
        Node* op_node = NodeCtor(OPERATOR, POW, 0, left_node, right_node);

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
    Node* new_node = NodeCtor(node->type, node->value, node->num_value);
    if (node->left != NULL) new_node->left = Copy_SubTree(node->left);
    if (node->right != NULL) new_node->right = Copy_SubTree(node->right);
    if (node->left != NULL) new_node->left->parent = new_node;
    if (node->right != NULL) new_node->right->parent = new_node;
    return new_node;
}

Node* Differenciate_Node(Node* node)
{
    extern int image_num;
    printf("I'm in differenciator, now dump%d\n", image_num);
    Tree_Dump(node, "differenciator");
    
    Nodes_To_Differenciate();

    switch (node->type)
    {
    case NUMBER:
        new_node = MAKE_ZERO_NODE();
        break;

    case VARIABLE:
        new_node = MAKE_ONE_NODE();
        break;

    case OPERATOR:
        switch (node->value)
        {
        case PLUS:
            new_node = MAKE_PLUS(DIFF_LEFT(), DIFF_RIGHT());             //NodeCtor(OPERATOR, PLUS, 0, Differenciate_Node(node->left), Differenciate_Node(node->right));
            break;
        
        case MINUS:
            new_node = MAKE_MINUS(DIFF_LEFT(), DIFF_RIGHT());             //NodeCtor(OPERATOR, MINUS, 0, Differenciate_Node(node->left), Differenciate_Node(node->right));
            break;

        case DIV:
            /*
            node_chislitel_1 = NodeCtor(OPERATOR, MUL, 0, Differenciate_Node(node->left), Copy_SubTree(node->right));
            node_chislitel_2 = NodeCtor(OPERATOR, MUL, 0, Copy_SubTree(node->left), Differenciate_Node(node->right));
            chislitel        = NodeCtor(OPERATOR, MINUS, 0, node_chislitel_1, node_chislitel_2);
            znamenatel       = NodeCtor(OPERATOR, MUL, 0, Copy_SubTree(node->right), Copy_SubTree(node->right));
            new_node         = NodeCtor(OPERATOR, DIV, 0, chislitel, znamenatel);
            */
            new_node = MAKE_DIV(MAKE_MINUS(MAKE_MUL(DIFF_LEFT(), COPY_RIGHT()), MAKE_MUL(DIFF_RIGHT(), COPY_LEFT())), MAKE_MUL(COPY_LEFT(), COPY_RIGHT()));
            break;

        case MUL:
            printf("meow mull\n");
            /*
            part_1           = NodeCtor(OPERATOR, MUL, 0, Differenciate_Node(node->left), Copy_SubTree(node->right));
            part_2           = NodeCtor(OPERATOR, MUL, 0, Copy_SubTree(node->left), Differenciate_Node(node->right));
            new_node         = NodeCtor(OPERATOR, PLUS, 0, part_1, part_2);
            */
            new_node = MAKE_PLUS(MAKE_MUL(DIFF_LEFT(), COPY_RIGHT()), MAKE_MUL(COPY_LEFT(), DIFF_RIGHT()));
            break;

        case SIN:
            //cos_node         = NodeCtor(OPERATOR, COS, 0, NULL, Copy_SubTree(node->right));
            //new_node         = NodeCtor(OPERATOR, MUL, 0, cos_node, Differenciate_Node(node->right));
            new_node = MAKE_MUL(MAKE_COS(COPY_RIGHT()), DIFF_RIGHT());
            break;

        case COS:
            //sin_node         = NodeCtor(OPERATOR, SIN, 0, NULL, Copy_SubTree(node->right));
            //left_part        = NodeCtor(OPERATOR, MUL, 0, NodeCtor(NUMBER, NUMBER, -1), sin_node);
            //new_node         = NodeCtor(OPERATOR, MUL, 0, left_part, Differenciate_Node(node->right));
            new_node = MAKE_MUL(MAKE_MUL(MAKE_MINUS_ONE_NODE(), MAKE_SIN(COPY_RIGHT())), DIFF_RIGHT());
            break;

        case TG:
            /*
            cos_node_1       = NodeCtor(OPERATOR, COS, 0, NULL, Copy_SubTree(node->right));
            cos_node_2       = NodeCtor(OPERATOR, COS, 0, NULL, Copy_SubTree(node->right));

            square           = NodeCtor(OPERATOR, MUL, 0, cos_node_1, cos_node_2);
            out_function     = NodeCtor(OPERATOR, DIV, 0, NodeCtor(NUMBER, NUMBER, 1), square);
            new_node         = NodeCtor(OPERATOR, MUL, 0, out_function, Differenciate_Node(node->right));
            */

            new_node = MAKE_MUL(MAKE_DIV(MAKE_ONE_NODE(), MAKE_MUL(COPY_RIGHT(), COPY_RIGHT())), DIFF_RIGHT());
            break;

        case CTG:
            /*
            sin_node_1       = NodeCtor(OPERATOR, SIN, 0, NULL, Copy_SubTree(node->right));
            sin_node_2       = NodeCtor(OPERATOR, SIN, 0, NULL, Copy_SubTree(node->right));
            
            square           = NodeCtor(OPERATOR, MUL, 0, sin_node_1, sin_node_2);
            left             = NodeCtor(OPERATOR, MUL, 0, NodeCtor(NUMBER, NUMBER, -1), square);
            out_function     = NodeCtor(OPERATOR, DIV, 0, NodeCtor(NUMBER, NUMBER, 1), left);
            new_node         = NodeCtor(OPERATOR, MUL, 0, out_function, Differenciate_Node(node->right));
            */
            new_node = MAKE_MUL(MAKE_DIV(MAKE_MINUS_ONE_NODE(), MAKE_MUL(COPY_RIGHT(), COPY_RIGHT())), DIFF_RIGHT());
            break;
        
        case EXP:
            new_node = MAKE_MUL(COPY(), DIFF_RIGHT());                   //= NodeCtor(OPERATOR, MUL, 0, Copy_SubTree(node->right), Differenciate_Node(node->right));
            break;
        
        case POW:
            EASY_PEASY(&node);
            if (IS_RIGHT_NUM)
            {
                //new_node = NodeCtor(OPERATOR, MUL, 0, NodeCtor(NUMBER, 0, node->right->num_value), NodeCtor(OPERATOR, POW, 0, Copy_SubTree(node->left), NodeCtor(NUMBER, 0, node->right->num_value - 1)));
                new_node = MAKE_MUL(MAKE_NUM(RIGHT_VALUE), MAKE_POW(COPY_LEFT(), MAKE_NUM(RIGHT_VALUE - 1)));
            }

            else if (IS_LEFT_NUM)
            {
                //new_node = NodeCtor(OPERATOR, MUL, 0, Differenciate_Node(node->right), NodeCtor(OPERATOR, POW, 0, Copy_SubTree(node->left), NodeCtor(OPERATOR, MINUS, 0, Copy_SubTree(node->right), NodeCtor(NUMBER, 0, 1))));
                new_node = MAKE_MUL(DIFF_LEFT(), MAKE_POW(COPY_LEFT(), MAKE_MINUS(COPY_RIGHT(), MAKE_ONE_NODE())));
            }
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
    Tree_Dump(proizvodnaya.root, "differentiate tree");
    EASY_PEASY(&proizvodnaya.root);
    Tree_Dump(proizvodnaya.root, "hahahhahaahah loh");
    return proizvodnaya;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void Replace_Nodes(Node* old_node, Node* new_node)
{
    if (old_node->parent != NULL)
    {
        new_node->parent = old_node->parent;
        if (old_node->parent->left == old_node) old_node->parent->left = new_node;
        else old_node->parent->right = new_node;
    }
    Node_Destructor(old_node);

}

bool EASY_PEASY(Node** NODE1, int rec_depth)
{
    Node* node = *NODE1;
    rec_depth++;
    printf("ayayayaya\n");
    Tree_Dump(node, "Easy pusy in");
    int c = 0;
    bool while_has_changed = true;

    while (while_has_changed)
    {
        c++;
        while_has_changed = false;
        Node* replace_node = NULL;


        if (node->type == OPERATOR)
        {
            switch (node->value)
            {
            case PLUS:
                COMMON_OPERATORS_COMMON_ACTIONS();
                if (IS_LEFT_NUM && node->left->num_value == 0)
                {
                    replace_node = node->right;
                    NODE_REPLACE();
                    while_has_changed = true;
                    break;
                }
                if (IS_RIGHT_NUM && node->right->num_value == 0)
                {
                    replace_node = node->left;
                    NODE_REPLACE();
                    while_has_changed = true;
                    break;
                }
                break;

            case MINUS:
                COMMON_OPERATORS_COMMON_ACTIONS();
                if (IS_RIGHT_NUM && node->right->num_value == 0)
                {
                    replace_node = node->left;
                    NODE_REPLACE();
                    while_has_changed = true;
                    break;
                }
                break;

            case MUL:
                COMMON_OPERATORS_COMMON_ACTIONS();


                if ((IS_LEFT_NUM && node->left->num_value == 0) || (IS_RIGHT_NUM && node->right->num_value == 0)) 
                {
                    replace_node = NodeCtor(NUMBER, 0, 0);
                    NODE_REPLACE();
                    while_has_changed = true; 
                    break;
                }
                if (IS_LEFT_NUM && node->left->num_value == 1)
                {
                    replace_node = node->right;
                    NODE_REPLACE();
                    while_has_changed = true; 
                    break;
                }

                if (IS_RIGHT_NUM && node->right->num_value == 1)
                {
                    replace_node = node->left;
                    NODE_REPLACE();
                    while_has_changed = true; 
                    break;
                }
                break;

            case POW:
                COMMON_OPERATORS_COMMON_ACTIONS();

                if (IS_RIGHT_NUM && IS_RIGHT_ONE)
                {
                    replace_node = node->left;
                    NODE_REPLACE();
                    while_has_changed = true;
                    break;
                }

                if (IS_RIGHT_NUM && IS_RIGHT_ZERO)
                {
                    replace_node = MAKE_ONE_NODE();
                    NODE_REPLACE();
                    while_has_changed = true;
                    break;
                }
                break;
            
            case DIV:
                COMMON_OPERATORS_COMMON_ACTIONS();

                if (IS_LEFT_NUM && node->left->num_value == 0)
                {
                    replace_node = NodeCtor(NUMBER, NUMBER, 0);
                    NODE_REPLACE();
                    while_has_changed = true; 
                    break;
                }

                if (IS_RIGHT_NUM && node->right->num_value == 1)
                {
                    replace_node = node->left;
                    NODE_REPLACE();
                    while_has_changed = true; 
                    break;
                }
                break;

            case SIN:
                if (IS_RIGHT_NUM) 
                {
                    replace_node = NodeCtor(NUMBER, 0, sin(node->right->num_value));
                    NODE_REPLACE();
                    while_has_changed = true; 
                    break;
                }
                while_has_changed += EASY_PEASY(&node->right, rec_depth);
                break;
            case COS:
                if (IS_RIGHT_NUM) 
                {
                    replace_node = NodeCtor(NUMBER, 0, cos(node->right->num_value));
                    NODE_REPLACE();
                    while_has_changed = true; 
                    break;
                }
                while_has_changed += EASY_PEASY(&node->right, rec_depth);
                break;
            case TG:
                if (IS_RIGHT_NUM) 
                {
                    replace_node = NodeCtor(NUMBER, 0, tan(node->right->num_value));
                    NODE_REPLACE();
                    while_has_changed = true; 
                    break;
                }
                while_has_changed += EASY_PEASY(&node->right, rec_depth);
                break;
            case CTG:
                if (IS_RIGHT_NUM) 
                {
                    replace_node = NodeCtor(NUMBER, 0, 1/tan(node->right->num_value));
                    NODE_REPLACE();
                    while_has_changed = true; 
                    break;
                }
                while_has_changed += EASY_PEASY(&node->right, rec_depth);
                break;
            case EXP:
                if (IS_RIGHT_NUM) 
                {
                    replace_node = NodeCtor(NUMBER, 0, exp(node->right->num_value));
                    NODE_REPLACE();
                    while_has_changed = true; 
                    break;
                }
                while_has_changed += EASY_PEASY(&node->right, rec_depth);
                break;

            default:
                break;
            }
        }
    }

    if (rec_depth == 1) *NODE1 = node;
    Tree_Dump(node, "Easy pusy out");
    printf("уауауауауауа\n");
    if (c > 1) return true;
    else return false;
}


void Change_Vars(Node* node, double x)
{
    Node* replace_node = NULL;
    if (IS_VAR) 
    {
        replace_node = NodeCtor(NUMBER, 0, x);
        NODE_REPLACE();
    }

    if (HAS_LEFT_CHILD) Change_Vars(node->left, x);
    if (HAS_RIGHT_CHILD) Change_Vars(node->right, x);
}

double Count_Value(Tree* tree, double x)
{
    Node* new_node = Copy_SubTree(tree->root);
    Change_Vars(new_node, x);
    Tree_Dump(new_node, "Count value");
    EASY_PEASY(&new_node);
    Tree_Dump(new_node, "Count value_gg");
    double g = new_node->num_value;
    Node_Destructor(new_node);
    return g;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

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
    printf("I've built the tree\n");
    Tree_Dump(tree->root, "TreeCtor");
    EASY_PEASY(&tree->root);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    setlocale(LC_ALL, "Rus");
    Tree tree = {};
    Differenciator_TreeCtor(&tree);
    Tree alpha_shtrih = Differenciate_Tree(&tree);
    Tree_Dump(tree.root, "I didn't change anything");
    printf("i'm here");
    double x = Count_Value(&tree, 1);
    double x_shtrih = Count_Value(&alpha_shtrih, 1);
    printf("function value is %lf, proizvodnoi is %lf\n", x, x_shtrih);

    Tex_Dump(&tree, 5);
}