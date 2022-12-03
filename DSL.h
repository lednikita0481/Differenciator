


#define IS_LEFT_OP        node->left->type == OPERATOR
#define IS_LEFT_PLUS      node->left->value == PLUS
#define IS_LEFT_MINUS     node->left->value == MINUS
#define IS_LEFT_MUL       node->left->value == MUL
#define IS_LEFT_DIV       node->left->value == DIV
#define IS_RIGHT_PLUS     node->right->value == PLUS
#define IS_RIGHT_MINUS    node->right->value == MINUS
#define IS_RIGHT_MUL      node->right->value == MUL
#define IS_RIGHT_DIV      node->right->value == DIV
#define IS_RIGHT_OP       node->right->type == OPERATOR
#define IS_LEFT_NUM       node->left->type == NUMBER
#define IS_RIGHT_NUM      node->right->type == NUMBER
#define IS_LEFT_VAR       node->left->type == VARIABLE
#define IS_RIGHT_VAR      node->right->type == VARIABLE
#define IS_VAR            node->type == VARIABLE
#define IS_NUM            node->type == NUMBER
#define IS_VAR            node->type == VARIABLE
#define IS_UN             (node->value == SIN || node->value == COS) && node->type == OPERATOR
#define IS_OP             node->type == OPERATOR
#define HAS_LEFT_CHILD    node->left != NULL
#define HAS_RIGHT_CHILD   node->right != NULL
#define HAS_PARENT        node->parent != NULL
#define IS_RIGHT_ONE      node->right->num_value == 1
#define IS_LEFT_ONE       node->left->num_value == 1
#define IS_RIGHT_ZERO     node->right->num_value == 0
#define IS_LEFT_ZERO      node->left->num_value == 0
#define MAKE_ZERO_NODE()  NodeCtor(NUMBER, 0, 0)
#define MAKE_ONE_NODE()   NodeCtor(NUMBER, 0, 1)
#define MAKE_MINUS_ONE_NODE() NodeCtor(NUMBER, 0, -1)

#define MAKE_PLUS(node1, node2)         NodeCtor(OPERATOR, PLUS, 0, node1, node2)
#define MAKE_MINUS(node1, node2)        NodeCtor(OPERATOR, MINUS, 0, node1, node2)
#define MAKE_MUL(node1, node2)          NodeCtor(OPERATOR, MUL, 0, node1, node2)
#define MAKE_DIV(node1, node2)          NodeCtor(OPERATOR, DIV, 0, node1, node2)
#define MAKE_SIN(node1)                 NodeCtor(OPERATOR, SIN, 0, NULL, node1)
#define MAKE_COS(node1)                 NodeCtor(OPERATOR, COS, 0, NULL, node1)
#define MAKE_POW(node1, node2)          NodeCtor(OPERATOR, POW, 0, node1, node2)
#define MAKE_NUM(num)                   NodeCtor(NUMBER, 0, num)
#define DIFF_LEFT()                     Differenciate_Node(node->left)
#define DIFF_RIGHT()                    Differenciate_Node(node->right)
#define COPY_LEFT()                     Copy_SubTree(node->left)
#define COPY_RIGHT()                    Copy_SubTree(node->right)
#define COPY()                          Copy_SubTree(node)

#define RIGHT_VALUE                     node->right->num_value

#define COMMON_OPERATORS_COMMON_ACTIONS()                                                       \
    if (node->left->type == NUMBER && node->right->type == NUMBER)                              \
    {                                                                                           \
        switch (node->value)                                                                    \
        {                                                                                       \
        case PLUS:                                                                              \
            replace_node = NodeCtor(NUMBER, 0, node->left->num_value + node->right->num_value); \
            break;                                                                              \
        case MINUS:                                                                             \
            replace_node = NodeCtor(NUMBER, 0, node->left->num_value - node->right->num_value); \
            break;                                                                              \
        case MUL:                                                                               \
            replace_node = NodeCtor(NUMBER, 0, (node->left->num_value) * (node->right->num_value));\
            break;                                                                              \
        case DIV:                                                                               \
            replace_node = NodeCtor(NUMBER, 0, node->left->num_value / node->right->num_value); \
            break;                                                                              \
        case POW:                                                                               \
            replace_node = NodeCtor(NUMBER, 0, pow(node->left->num_value, node->right->num_value));\
            break;                                                                              \
        default:                                                                                \
            break;                                                                              \
        }                                                                                       \
        Replace_Nodes(node, replace_node);                                                      \
        node = replace_node;                                                                    \
        while_has_changed = true;                                                               \
        break;                                                                                  \
    }                                                                                           \
    if (IS_LEFT_OP) while_has_changed += EASY_PEASY(&node->left, rec_depth);                    \
    if (IS_RIGHT_OP) while_has_changed += EASY_PEASY(&node->right, rec_depth);             


#define NODE_REPLACE()                  \
    Replace_Nodes(node, replace_node);  \
    node = replace_node;                              
