#include "codeproc.h"
//заполняю структуру листа(символ и его частота)
Node* createLeafNode(unsigned char symbol, unsigned int freq){
    Node* node = (Node*)malloc(sizeof(Node));
    node ->symbol = symbol;
    node ->freq = freq;
    node -> left = NULL;
    node -> right = NULL;
    return node;//возвращаю структуру!
}
//теперь создаю связь из листьев или из других связей, которые оказались ниже.
Node* createInternalNode(Node* left, Node* right){
    Node* node = (Node*)malloc(sizeof(Node));
    node->symbol = '$';
    node -> freq = left->freq + right->freq;
    node ->left = left;
    node -> right = right;
    return node;
}