#include "codeproc.h"

//считаю уникальные символы
Node** createNodeArray(unsigned int* freq, int* count){
    *count = 0;
    for (int i = 0; i<256; i++){
        if (freq[i]>0){
            (*count)++;
        }
    }

    //создал массив указателей на узлы
    Node** nodes = (Node**)malloc(*count * sizeof(Node*));
    int index = 0;

    //теперь заполняю его листьями
    for (int i = 0; i < 256; i++){
        unsigned char ch = i;
        if (freq[i]>0){
            nodes[index] = createLeafNode(ch,freq[i]);
            index++;
        }
    }

    return nodes;
}

//сортирую по возрастанию
void buildMinHeap(Node** nodes, int count){
    for (int i = 0; i < count-1; i++){
        for (int j = i + 1; j < count; j++){
            if (nodes[j]->freq < nodes[i]->freq){
                Node* temp = nodes[i];
                nodes[i] = nodes[j];
                nodes[j] = temp;
            }
        }
    }
}

void insertToHeap(Node** nodes, int* count, Node* newNode) {
    nodes[*count] = newNode;
    (*count)++;
    buildMinHeap(nodes, *count); // Пересортируем
}

Node* extractMin(Node** nodes, int* count){
    if (*count == 0) return NULL;

    Node* min = nodes[0];

    for (int i = 0; i <*count - 1; i++){
        nodes[i] = nodes[i+1];
    }

    (*count)--;//вычитаю т.к. взял два листа(как бы вычитаю из общего количества) и потом в tree.c добавлю (nodeCount++) узел новообразовавшийся 
    return min;
}