typedef struct Node{
    unsigned char symbol;
    unsigned int freq;
    struct Node *left;
    struct Node *right;
} Node;