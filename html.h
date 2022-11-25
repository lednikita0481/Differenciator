#include <stdio.h>
#include <stdlib.h>

int image_num = 0;
const char png[] = ".png";

#ifndef HTML_NAME
    #define HTML_NAME "dump.html"
#endif


FILE* Open_HTML();
void Close_HTML();

FILE* HTML = Open_HTML();

FILE* Open_HTML() {
    FILE* gg = fopen(HTML_NAME, "w");
    atexit(Close_HTML);
    return gg;
}

void Close_HTML() 
{
    fclose(HTML);
}

void Add_To_HTML(char* image)
{
    fprintf(HTML, "<img src = \"%s\"><hr>", image);
    fflush(HTML);
}