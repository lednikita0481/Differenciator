static FILE* Open_Tex();
static void Close_Tex();
static const char* Tex_Value_Name(Node* node);
static void Tex_Recursion(Node* node);
void Tex_Dump(Tree* tree, int n, double left_x, double right_x); 