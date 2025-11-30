#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Node_tree.h"

Node* createLeafNode(unsigned char, unsigned int);

Node* createInternalNode(Node*, Node*);

void countFrequencies(FILE*, unsigned int*);