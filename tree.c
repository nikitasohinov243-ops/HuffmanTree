#include "codeproc.h"
#include "Min-Heap.h"

int main(int argn, char *args[]){
    if (argn < 3){
        printf("Wrong arguments! Usage:\n");
        printf("  Encode: %s -e input.txt output.huff\n", args[0]);
        printf("  Decode: %s -d input.huff output.txt\n", args[0]);
        return 1;
    }

    FILE *encode_file = fopen(args[1], "rb");
    FILE *decode_file = fopen(args[2], "wb");

    if (encode_file == NULL){
        printf("Unable to open input file: %s\n", args[1]);
        return 1;
    }

    unsigned int freq[256];
    //частота
    countFrequencies(encode_file, freq);

    //куча из узлов
    int nodeCount;
    Node** nodes = createNodeArray(freq, &nodeCount);
    buildMinHeap(nodes, nodeCount);

    while (nodeCount > 1) {
        Node* left = extractMin(nodes,&nodeCount);
        Node* right = extractMin(nodes, &nodeCount);
        Node* parent = createInternalNode(left, right);
        insertToHeap(nodes,&nodeCount,parent);
    }

    Node* root = nodes[0];
    printf("Дерево построено! Корневая частота: %d\n", root->freq);

    fclose(encode_file);
    return 0;
}

//gcc -o huffman tree.c node_leaf.c min-heap.c freq.c