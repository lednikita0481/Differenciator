#include <stdio.h>
#include <stdlib.h>

//#include "Tex.h"
#include "Tree.h"
#include "Differenciator.h"
#include "DSL.h"

const char* TEX_START =   "\\documentclass[a4paper, 12pt]{article}\n"
                            "\\usepackage[a4paper,top=1.5cm, bottom=1.5cm, left=1cm, right=1cm]"
                            "{geometry}\n"
                            "\\usepackage[utf8]{inputenc}\n"
                            "\\usepackage{mathtext} \n"
                            "\\usepackage[english, russian]{babel} \n"
                            "\\title{��� �������, �� ������ �����, ��� � � �������� ���������, ��� �� ����� ����������� ������ � �����������-������������ �������� ���������� ���� ���������� �������� ��������}\n"
                            "\\author{������, �����-��������}\n"
                            "\\date{\\today}\n";


char* WORDS[] =  
{
    "���� ������ ���� ���������... � � �������� ���, � �����, 76834683908597050295839057683905 ��� ���� � �� ��",
    "�� �������� ��, ���",
    "��� ������, ��� � ��� ������? �� ����� ������ ��������, ������ �������� � �� ����� �����.",
    "���� �� �� �������� ���, �� �� �����, ���:",
    "�.�. ��������� ������ �������, ���:",
    "������������� �����! ��� ��������� ������, ��� � ������ ������� ���� ������, ����, ��� �����...",
    "�������� ����� �������� ���� ����� ����� �������� � ��������� �������. �� ������� ������."
    "��������, ������� �� ���� ���������� ��� ��� - ���� �� �������� ������������ ���������� ����� �������, ������� � �������� � ���� �������������� �� �������� �� ����� ��������� ����.",
    "�������� ����� ��������� ����������� ������ �� �������� ���� ������ ������������ �����. � �� ��������.",
    "����� ������ ��� ��� ������, ���� ����� ���� �����, � ��� 42"
};

#define Tex_Print(str...)                                                \
    fprintf(Tex_File, str);                                              \
    fflush(Tex_File);

FILE* Tex_File = NULL;

static FILE* Open_Tex() 
{
    FILE *Tex_File = fopen("texFile.tex", "w");
    fprintf(Tex_File, TEX_START);
    fprintf(Tex_File, "\\begin{document}\n");
    fprintf(Tex_File, "\\maketitle\n");
    return Tex_File;
}

static void Close_Tex() 
{
    fprintf(Tex_File, "\\end{document}");
    fclose(Tex_File);
}

static const char* Tex_Value_Name(Node* node)
{
    char* value_name = NULL;

    switch (node->type)
    {
    case NUMBER:
        printf("�� ��� �����, � ��� �� ����, ����� ��!\n");
        break;

    case VARIABLE:
        value_name = "x";
        break;
    
    case OPERATOR:
        switch (node->value)
        {
        case PLUS:
            value_name = "+{";
            break;

        case MINUS:
            value_name = "-{";
            break;

        case MUL:
            value_name = "\\cdot{";
            break;
        
        case SIN:
            value_name = "\\sin{";
            break;

        case COS:
            value_name = "\\cos";
            break;

        case TG:
            value_name = "\\tg{";
            break;

        case CTG:
            value_name = "\\ctg{";
            break;

        case EXP:
            value_name = "e^(";
            break;
        case POW:
            value_name = "^{";
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

static void Tex_Recursion(Node* node)
{
    if (IS_OP)
    {
        if (IS_UN)
        {
            Tex_Print(Tex_Value_Name(node));
            Tex_Print("(");
            Tex_Recursion(node->right);
            Tex_Print(")}");
        }
        else if (node->value == DIV)
        {
            Tex_Print("\\frac{");
            Tex_Recursion(node->left);
            Tex_Print("}{");
            Tex_Recursion(node->right);
            Tex_Print("}");
        }
        else
        {
            Tex_Recursion(node->left);
            Tex_Print(Tex_Value_Name(node));
            Tex_Recursion(node->right);
            Tex_Print("}");
        }
    }

    else if (IS_NUM)
    {
        if (node->num_value < 0) Tex_Print("(");
        Tex_Print("%.3lf", node->num_value);
        if (node->num_value < 0) Tex_Print(")");
    }
    
    else if (IS_VAR)
    {
        Tex_Print("x");
    }
}



void Tex_Dump(Tree* tree, int n)
{
    Node* node = tree->root;
    Tex_File = Open_Tex();
    Tex_Print("������ �������? ������ ������� ���?.. �������, ��� ������, ���� �� ������... � ��� ���������� ��� �������������!\n");
    Tex_Print("�����, ����� ��������, ��� �� ��� �����������\n");

    Tex_Print("\\begin{equation}\n");
    Tex_Print("f(x) = ");
    Tex_Recursion(node);
    Tex_Print("\\end{equation}\n");
    Tex_Print("\\par\n");

    Tex_Print("������ ������ ��� ������� ����� ���� 392058954437 ���. � � ��������� ����� ��� �������� ��� ����, ");
    Tex_Print("c��� ������� �������� �� ������ ������ �������� ���������� ��� �������� ����.\n");
    Tex_Print("�� ������ ���, ������, �� ����... ����, �������, ����� � �� ������� ������ �������� ��� ���� ������� �� ���������, ��� ���� �������...\n");

    Tex_Print("\\section{�����������}");
    Tex_Print("�� %d �����������? ��� ��� �� �������� ������������. ������ ���� �� �� %d\\par\n", n, n + (rand() % 10 + 10));

    int values[n] = {};
    Tree treas[n] = {};
    for (int i = 1; i <= n; i++)
    {
        if (i == 1) treas[i-1] = Differenciate_Tree(tree);
        else treas[i-1] = Differenciate_Tree(&treas[i-2]);

        Tex_Print(WORDS[rand()%9]);
        Tex_Print("\\begin{equation}\n");
        Tex_Print("f^{(%d)}(x) = ", i);
        Tex_Recursion(treas[i-1].root);
        Tex_Print("\\end{equation}\n\\par\n");

        values[i-1] = Count_Value(&treas[i-1], 0);

        Tex_Print("\\begin{equation}\n");
        Tex_Print("f^{(%d)}(0) = %lf", i, values[i-1]);
        Tex_Print("\\end{equation}\n\\par\n");
        Tex_Print("\\par\n");
    }

    Tex_Print("\\par\n");
    Tex_Print("�� �������������, ����� ������ ���� �������������. � ����, �� ���� ����������.\n");
    Tex_Print("\\par\n");
    Tex_Print("����, ���� ��� � �������� �����.... ���� ���������� ���� ����� �� ��� �� ������. �� �� ����������...");

    Tex_Print("\\section{��������}");
    Tex_Print("� ����������� � ��� ��������� �� ��������� 1000 ��� ����� ���������� ��� ������ ������� ��������� ������ ���. ");
    Tex_Print("������ ������ ��������� ��� ���� ������ ��������. ������ ������ ��������� ���� �� �����. ������ ������ ��������� �� ��������� ��� �� ��������� ������������. ����� ����� � ������������ �����������\n");
    Tex_Print("�� �� ���� ���������� � ������ ������� �� ���������. �� ��� ��� ��������, � �� ����� ���� �����, ������ ��� �����\n");
    Tex_Print("\\par\n");

    Tex_Print("\\begin{equation}\n");
    Tex_Print("f(x) = %lf +", Count_Value(tree, 0));
    for (int i = 1; i <= n; i++)
    {
        Tex_Print(" + \\frac{%.1lf \\cdot x^{%d}}{%d!}", values[i-1], i, i);
    }
    Tex_Print(" +o(x^{%d})", n);
    Tex_Print("\\end{equation}\n");

    Tex_Print("�� ������, �� ���� ��� ������� � ������������ �� ����. ���� � � ��������� ��� ����� ����� ������������ ������, ������� ���� �������, ");
    Tex_Print("� ��������� � ����� �������� �� ���������. ��, ���� � ������ ��������, � ��� ������� ���� �������� - ��������������, ");
    Tex_Print("��������� ����� ����� ���������, � ����� ��� �� ����� �� ������� ������ �����, ��������� � ����� ������, ")
    Tex_Print("�� ����� � ����������.... � ����� ... ��� ��� ������ ���� ����������...")
    Tex_Print("\\par\n");
    Tex_Print("�� ��������, � may the source be with you!");
    Close_Tex();
}