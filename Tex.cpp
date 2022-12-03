#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#include "Tex.h"
#include "Tree.h"
#include "Differenciator.h"
#include "DSL.h"

const char* TEX_START =   "\\documentclass[a4paper, 12pt]{article}\n"
                            "\\usepackage[a4paper,top=1.5cm, bottom=1.5cm, left=1cm, right=1cm]"
                            "{geometry}\n"
                            "\\usepackage[cp1251]{inputenc}\n"
                            "\\usepackage{mathtext} \n"
                            "\\usepackage[english, russian]{babel} \n"
                            //"\\usepackage{cmap}\n"
                            "\\usepackage{graphicx}\n"
                            //"\\usepackage{epstopdf}\n"
                            "\\usepackage{tikz}\n"
                            "\\usepackage{pgfplots}"
                            "\\usepackage[export]{adjustbox}\n"
                            "\\usepackage{pgfplotstable}\n"
                            "\\usepackage{float}\n"
                            "\\usepackage{wrapfig}\n"
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
    "�������� ����� �������� ���� ����� ����� �������� � ��������� �������. �� ������� ������.",
    "��������, ������� �� ���� ���������� ��� ��� - ���� �� �������� ������������ ���������� ����� �������, ������� � �������� � ���� �������������� �� �������� �� ����� ��������� ����.",
    "�������� ����� ��������� ����������� ������ �� �������� ���� ������ ������������ �����. � �� ��������.",
    "����� ������ ��� ��� ������, ���� ����� ���� �����, � ��� 42"
};

#define Tex_Print(str...)                                                \
    fprintf(Tex_File, str);                                              \
    fflush(Tex_File);
#define Plot_Print(str...)                                                \
    fprintf(Plot_File, str);                                              \
    fflush(Plot_File);

FILE* Tex_File = NULL;
FILE* Plot_File = NULL;

static FILE* Open_Tex() 
{
    FILE *Tex_File = fopen("Tex_File.tex", "w");
    fprintf(Tex_File, TEX_START);
    fprintf(Tex_File, "\\begin{document}\n");
    fprintf(Tex_File, "\\maketitle\n");
    return Tex_File;
}

static void Close_Tex() 
{
    fprintf(Tex_File, "\\end{document}");
    fclose(Tex_File);
    system("pdflatex Tex_File.tex >/dev/null");
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
            value_name = " + {";
            break;

        case MINUS:
            value_name = " - {";
            break;

        case MUL:
            value_name = "\\cdot{";
            break;
        
        case SIN:
            value_name = "\\sin{";
            break;

        case COS:
            value_name = "\\cos{";
            break;

        case TG:
            value_name = "\\tg{";
            break;

        case CTG:
            value_name = "\\ctg{";
            break;

        case EXP:
            value_name = "e^{";
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

static const char* Plot_Value_Name(Node* node)
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
            value_name = "+(";
            break;

        case MINUS:
            value_name = "-(";
            break;

        case MUL:
            value_name = "*(";
            break;
        
        case SIN:
            value_name = "np.sin(";
            break;

        case COS:
            value_name = "np.cos(";
            break;

        case TG:
            value_name = "np.tg(";
            break;

        case CTG:
            value_name = "np.ctg(";
            break;

        case EXP:
            value_name = "np.exp(";
            break;
        case POW:
            value_name = "**(";
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
        else if (node->value == POW)
        {
            Tex_Print("(");
            Tex_Recursion(node->left);
            Tex_Print(")^{");
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
        Tex_Print("%lg", node->num_value);
        if (node->num_value < 0) Tex_Print(")");
    }
    
    else if (IS_VAR)
    {
        Tex_Print("x");
    }
}

void Recursive_Plot(Node* node)
{
    if (IS_OP)
    {
        if (IS_UN)
        {
            Plot_Print(Plot_Value_Name(node));
            Recursive_Plot(node->right);
            Plot_Print(")");
        }
        else if (node->value == DIV)
        {
            Plot_Print("(");
            Recursive_Plot(node->left);
            Plot_Print(")/(");
            Recursive_Plot(node->right);
            Plot_Print(")");
        }
        else
        {
            Recursive_Plot(node->left);
            Plot_Print(Plot_Value_Name(node));
            Recursive_Plot(node->right);
            Plot_Print(")");
        }
    }
    else if (IS_NUM)
    {
        if (node->num_value < 0) Plot_Print("(");
        Plot_Print("%lg", node->num_value);
        if (node->num_value < 0) Plot_Print(")");
    }
    
    else if (IS_VAR)
    {
        Plot_Print("x");
    }
}

void Make_Plot(Tree* tree, Tree* derivative, double kas_dot, double left_x, double right_x)
{
    Plot_File = fopen("plot.py", "w\n");
    Plot_Print("import numpy as np\n");
    Plot_Print("import matplotlib.pyplot as plt\n");
    Plot_Print("x = np.linspace(%lf, %lf, 200)\n", left_x, right_x);
    Plot_Print("y = ");
    Recursive_Plot(tree->root);
    Plot_Print("\n");

    Plot_Print("plt.figure(figsize=(8,6), dpi=100)\n");
    Plot_Print("plt.grid(True, linestyle=\"--\")\n");
    Plot_Print("plt.axis([%lf, %lf, np.min(y), np.max(y)])\n", left_x, right_x);
    Plot_Print("plt.plot(x, y, \"-m\",linewidth=0.5)\n");
    double aiaiai = Count_Value(derivative, kas_dot);
    Plot_Print("y = %d * x + %d\n", aiaiai, (Count_Value(tree, kas_dot) - ((aiaiai)*kas_dot)));
    Plot_Print("plt.plot(x, y, \"-g\",linewidth=0.4)\n");
    //Plot_Print("plt.show()\n");
    Plot_Print("plt.savefig('plot.png')");
    fclose(Plot_File);
    system("python plot.py");
}



void Tex_Dump(Tree* tree, int n, double left_x, double right_x)
{
    Node* node = tree->root;
    Tex_File = Open_Tex();
    Tex_Print("������ �������? ������ ������� ���?.. �������, ��� ������, ���� �� ������... � ��� ���������� ��� �������������!\n");
    Tex_Print("�����, ����� ��������, ��� �� ��� �����������\n");

    Tex_Print("$ ");
    Tex_Print("f(x) = ");
    Tex_Recursion(node);
    Tex_Print("$\n");
    Tex_Print("\\par\n");

    Tex_Print("������ ������ ��� ������� ����� ���� 392058954437 ���. � � ��������� ����� ��� �������� ��� ����, ");
    Tex_Print("c��� ������� �������� �� ������ ������ �������� ���������� ��� �������� ����.\n");
    Tex_Print("�� ������ ���, ������, �� ����... ����, �������, ����� � �� ������� ������ �������� ��� ���� ������� �� ���������, ��� ���� �������...\n");

    Tex_Print("\\section{�����������}");
    Tex_Print("�� %d �����������? ��� ��� �� �������� ������������. ������ ���� �� �� %d\\par\n", n, n + (rand() % 10 + 10));

    double values[n] = {};
    Tree treas[n] = {};

    long ltime = time(NULL);
    int stime = (unsigned int) ltime/2;
    srand(stime);

    for (int i = 1; i <= n; i++)
    {
        if (i == 1) treas[i-1] = Differenciate_Tree(tree);
        else treas[i-1] = Differenciate_Tree(&treas[i-2]);

        Tex_Print(WORDS[rand()%10]);
        Tex_Print("\n\\par\n");
        Tex_Print("$ ");
        Tex_Print("f^{(%d)}(x) = ", i);
        Tex_Recursion(treas[i-1].root);
        Tex_Print("$\n\\par\n");

        values[i-1] = Count_Value(&treas[i-1], 0);
        Tex_Print("$ ");
        Tex_Print("f^{(%d)}(0) = %lf", i, values[i-1]);
        Tex_Print("$\n\\par\n");
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

    Tex_Print("$ ");
    Tex_Print("f(x) = %lf +", Count_Value(tree, 0));
    for (int i = 1; i <= n; i++)
    {
        if (values[i-1] != 0) Tex_Print(" + \\frac{%.1lf \\cdot x^{%d}}{%d!}", values[i-1], i, i);
    }
    Tex_Print(" +o(x^{%d})", n);
    Tex_Print("$\n");

    Make_Plot(tree, &treas[0], 4, left_x, right_x);

    Tex_Print("\\section{������}");
    Tex_Print("��� ��� ��� ����� �� ���� �����? ������? �� ������ ������ � ������������� �������� ����������� �������� �����? ������ ��� ���� ���������?\n");
    Tex_Print("\\par\n");
    Tex_Print("\\begin{figure}[h]\n");
    Tex_Print("\\center{\\includegraphics[width=0.7\\linewidth]{plot.png}}\n");
    Tex_Print("\\caption{��� ���� ������}\n");
    Tex_Print("\\label{ris:image}\n");
    Tex_Print("\\end{figure}\n");

    Tex_Print("�� ������, �� ���� ��� ������� � ������������ �� ����. ���� � � ��������� ��� ����� ����� ������������ ������, ������� ���� �������, ");
    Tex_Print("� ��������� � ����� �������� �� ���������. ��, ���� � ������ ��������, � ��� ������� ���� �������� - ��������������, ");
    Tex_Print("��������� ����� ����� ���������, � ����� ��� �� ����� �� ������� ������ �����, ��������� � ����� ������, ")
    Tex_Print("�� ����� � ����������.... � ����� ... ��� ��� ������ ���� ����������...")
    Tex_Print("\\par\n");
    Tex_Print("�� ��������, � may the source be with you!");
    for (int i = 0; i < n; i++) Nodes_Destructor(treas[i].root);
    Close_Tex();
}