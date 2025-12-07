#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node{
    unsigned char symbol;
    unsigned int freq;
    struct Node *left;
    struct Node *right;
} Node;

// Прототипы функций
Node* createLeafNode(unsigned char, unsigned int);
Node* createInternalNode(Node*, Node*);
void countFrequencies(FILE*, unsigned int*);
void generateCodes(Node*, char*, int, char codes[256][256]);
void freeTree(Node*);
int compareFiles(FILE *, FILE *);
Node* rebuildTreeFromFreq(unsigned int freq[256]);

void encodeFileBit(FILE*, FILE*, char codes[256][256]);
void decodeFileBit(FILE*, FILE*, Node*);

//Min-heap
Node** createNodeArray(unsigned int*, int*);
void buildMinHeap(Node**, int);
Node* extractMin(Node**, int*);
void insertToHeap(Node**, int*, Node*);