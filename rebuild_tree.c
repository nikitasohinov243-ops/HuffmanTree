#include "codeproc.h"

Node* rebuildTreeFromFreq(unsigned int freq[256]) {
    printf("Rebuilding Huffman tree from frequencies...\n");
    
    // 1. Создаем массив узлов из частот
    int nodeCount;
    Node** nodes = createNodeArray(freq, &nodeCount);
    
    if (nodeCount == 0) {
        printf("Warning: No symbols with non-zero frequency\n");
        free(nodes);
        return NULL; // Пустой файл
    }
    
    printf("Unique symbols: %d\n", nodeCount);
    
    // 2. Строим минимальную кучу
    buildMinHeap(nodes, nodeCount);
    
    // 3. Построение дерева Хаффмана
    while (nodeCount > 1) {
        Node* left = extractMin(nodes, &nodeCount);
        Node* right = extractMin(nodes, &nodeCount);
        Node* parent = createInternalNode(left, right);

        nodes[nodeCount] = parent;
        nodeCount++;
        buildMinHeap(nodes, nodeCount);
    }
    
    // 4. Корень дерева - единственный оставшийся узел
    Node* root = nodes[0];
    
    // 5. Освобождаем массив узлов (но не само дерево!)
    free(nodes);
    
    printf("Tree rebuilt successfully\n");
    return root;
}