#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>

#include "Tree.h"
#include "html.h"

int CELL = 0;

void TreeCtor(Tree* tree)
{
    tree->root = (Node*) calloc(1, sizeof(Node));
    tree->size = 1;
    //Tree_Dump(tree);
}

Node* NodeCtor(Node_Types type, int value, double num_value, Node* left, Node* right, Node* parent)
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));
    
    new_node->left = left;
    new_node->parent = parent;
    new_node->right = right;

    new_node->type = type;
    new_node->value = value;
    new_node->num_value = num_value;

    new_node->cell_num = CELL;
    CELL++;

    return new_node;
}

const char* Type_Name(Node_Types type)
{
    const char* type_name = NULL;

    switch (type)
    {
    case NUMBER:
        type_name = "Number";
        break;
    case VARIABLE:
        type_name = "Variable";
        break;
    case OPERATOR:
        type_name = "Operator";
        break;
    default:
        printf("ты втираешь мне какую-то дичь.\n");
        break;
    }

    return type_name;
}

const char* Value_Name(Node* node)
{
    char* value_name = NULL;

    switch (node->type)
    {
    case NUMBER:
        printf("Вы кто такие, я вас не звал, идите на!\n");
        break;

    case VARIABLE:
        value_name = "x";
        break;
    
    case OPERATOR:
        switch (node->value)
        {
        case PLUS:
            value_name = "+";
            break;

        case MINUS:
            value_name = "-";
            break;

        case MUL:
            value_name = "*";
            break;
        
        case DIV:
            value_name = "/";
            break;
        
        case SIN:
            value_name = "sin";
            break;

        case COS:
            value_name = "cos";
            break;

        case TG:
            value_name = "tg";
            break;

        case CTG:
            value_name = "ctg";
            break;

        case EXP:
            value_name = "exp";
            break;

        
        default:
            break;
        }

        break;
    
    default:
        break;
    }

    return value_name;
}

void Recursive_Node_Dump(Node* node, FILE* dump_file)
{
    char* color = NULL;
    char* edge_color = NULL;
    if (node->left == NULL && node->right == NULL) color = "darkolivegreen1";
    else  color = "cyan1";

    DUMP("cell_%d [label = \"%s|", node->cell_num, Type_Name(node->type));
    if (node->type == NUMBER)
    {
        DUMP("%lf", node->num_value);
    } 
    else 
    {
        DUMP("%s", Value_Name(node));
    }
    DUMP("\"\n style = filled\n fillcolor = %s \n]\n", color);

    if (node->parent != NULL)
    {
        if (node->parent->left == node) edge_color = "firebrick4";
        else edge_color = "darkgreen";
        
        DUMP("cell_%d->cell_%d[color = %s]\n", node->parent->cell_num, node->cell_num, edge_color);
    }

    if (node->left != NULL) Recursive_Node_Dump(node->left, dump_file);
    if (node->right != NULL) Recursive_Node_Dump(node->right, dump_file);
}

void Tree_Dump(Node* root_node, char* func)
{
    FILE* dump_file = fopen("dump_dot.dot", "w");                            
    DUMP("digraph{\n");                                                 
    DUMP("nodesep = 2;\n");                                             
    DUMP("splines = \"ortho\";\n");                                     
    DUMP("node [ shape=record, style = rounded, fontsize = 30];\n");  

    DUMP("cell_func [label = \"%s\"]", func);
    DUMP("cell_%d [label = \"%s|", root_node->cell_num, Type_Name(root_node->type));
    if (root_node->type == NUMBER)
    {
        DUMP("%lf", root_node->num_value);
    } 
    else 
    {
        DUMP("%s", Value_Name(root_node));
    }
    DUMP("\"\n style = filled\n fillcolor = violet ]\n");
    
    if (root_node->left != NULL) Recursive_Node_Dump(root_node->left, dump_file);
    if (root_node->right != NULL) Recursive_Node_Dump(root_node->right, dump_file);

    DUMP("}");

    fclose(dump_file);

    char command[100] = "dot -Tpng dump_dot.dot -Gcharset=latin1 -o ";
    char name[20] = "data";
    char num[10] = "";

    //itoa(image_num, num, 10);
    sprintf(num, "%d", image_num);
    strcat(num, png);
    strcat(name, num);
    strcat(command, name);

    image_num++;

    printf("\n\n\n\nI'm in here, command is %s\n\n\n\n\n", command);
    system(command);

    Add_To_HTML(name);
}