#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Node_tree.h"

// Прототипы функций
Node* createLeafNode(unsigned char, unsigned int);
Node* createInternalNode(Node*, Node*);
void countFrequencies(FILE*, unsigned int*);
void generateCodes(Node*, char*, int, char codes[256][256]);
void freeTree(Node*);
void decodeFile(FILE*, FILE*, Node*);
int compareFiles(FILE *, FILE *);