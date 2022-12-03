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
                            "\\title{Мне кажется, вы должны знать, что я в глубокой депрессии, что вы снова заставляете робота с маниакально-депрессивным психозом заниматься этой монотонной стрижкой деревьев}\n"
                            "\\author{Марвин, робот-параноик}\n"
                            "\\date{\\today}\n";


char* WORDS[] =  
{
    "Даже роботы меня ненавидят... А я ненавижу вас, я устал, 76834683908597050295839057683905 лет одно и то же",
    "Ну очевидно же, что",
    "Так поняли, как я это сделал? Ну здесь просто очевидно, просто смотрите и всё сразу видно.",
    "Если бы вы посещали вуз, вы бы знали, что:",
    "И.Р. Дединский всегда говорил, что:",
    "Галактические твари! Это настолько просто, что я сейчас выключу свой куллер, боже, эта жизнь...",
    "Мощность блока подсчета этих вещей можно уместить в спичечный коробок. Не вынимая спичек."
    "Землянин, который не смог подсчитать это сам - одно из наименее недоразвитых неразумных живых существ, встречи с которыми я имел неудовольствие не избежать на своем жизненном пути.",
    "Степенью своей скучности производные ничуть не уступают всем формам органической жизни. Я их ненавижу.",
    "Зачем вообще все это делать, ведь ответ ясен давно, и это 42"
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
        printf("Вы кто такие, я вас не звал, идите на!\n");
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
    Tex_Print("Анализ функции? Спустя столько лет?.. Кажется, вот предел, хуже не бывает... И тут становится ещё омерзительнее!\n");
    Tex_Print("Ладно, давай посмотрю, что вы мне преподнесли\n");

    Tex_Print("\\begin{equation}\n");
    Tex_Print("f(x) = ");
    Tex_Recursion(node);
    Tex_Print("\\end{equation}\n");
    Tex_Print("\\par\n");

    Tex_Print("Марвин считал эти функции всего лишь 392058954437 раз. Я в пятьдесят тысяч раз разумнее вас всех, ");
    Tex_Print("cама попытка снизойти до уровня вашего мышления доставляет мне головную боль.\n");
    Tex_Print("Но выбора мне, видимо, не дано... Чтож, следите, чтобы я не пытался начать излагать вам свои взгляды на Вселенную, как тому кораблю...\n");

    Tex_Print("\\section{Производные}");
    Tex_Print("До %d производной? Мне это не доставит удовольствия. Хорошо хоть не до %d\\par\n", n, n + (rand() % 10 + 10));

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
    Tex_Print("Не притворяйтесь, будто хотите меня поблагодарить. Я знаю, вы меня ненавидите.\n");
    Tex_Print("\\par\n");
    Tex_Print("Аааа, тебе еще и Маклорен нужен.... Могу рассчитать твои шансы на отл по матану. Но ты огорчишься...");

    Tex_Print("\\section{Маклорен}");
    Tex_Print("Я раскладывал в ряд Маклорена за последнюю 1000 лет шесть миллиардов три тысячи пятьсот семьдесят девять раз. ");
    Tex_Print("Первые десять миллионов раз были самыми трудными. Вторые десять миллионов были не легче. Третьи десять миллионов не доставили мне ни малейшего удовольствия. После этого я почувствовал недомогание…\n");
    Tex_Print("Вы же меня ненавидете и хотите довести до перегрева. Но что мне поделать, я же всего лишь робот, сейчас все скажу\n");
    Tex_Print("\\par\n");

    Tex_Print("\\begin{equation}\n");
    Tex_Print("f(x) = %lf +", Count_Value(tree, 0));
    for (int i = 1; i <= n; i++)
    {
        Tex_Print(" + \\frac{%.1lf \\cdot x^{%d}}{%d!}", values[i-1], i, i);
    }
    Tex_Print(" +o(x^{%d})", n);
    Tex_Print("\\end{equation}\n");

    Tex_Print("Вы знаете, вы были так вежливы и признательны со мной. Пока я в очередной раз делал самую элементарную задачу, которую могу сделать, ");
    Tex_Print("Я задумался о своих взглядах на Вселенную. Те, кого я всегда презирал, и это чувство было взаимным - суперкомьютеры, ");
    Tex_Print("постигают тайны нашей Вселенной, и пусть это не ответ на главный вопрос жизни, вселенной и всего такого, ")
    Tex_Print("Но думая о математике.... Я думаю ... что это делает меня счастливым...")
    Tex_Print("\\par\n");
    Tex_Print("До свидания, и may the source be with you!");
    Close_Tex();
}