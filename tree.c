#include "codeproc.h"
#include <string.h>

// /Users/nikitasohinov/Documents/ВШЭ/Лабы/programirovanie.2/labs/HuffmanTree
// ./huffman -e input.txt output.bin codes.txt
// ./huffman -d 
// gcc -o huffman tree.c node_leaf.c min-heap.c freq.c codes.c bit_work.c

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage:\n");
        printf("  Encode: %s -e input.txt output.bin codes.txt\n", argv[0]);
        printf("  Decode: %s -d input.bin output.txt codes.txt\n", argv[0]);
        return 1;
    }

    char* mode = argv[1];
    char* input_filename = argv[2];
    char* output_filename = argv[3];
    char* codes_filename = argv[4];

    if (strcmp(mode, "-e") == 0) {
        // ========== КОДИРОВАНИЕ ==========
        FILE *input_file = fopen(input_filename, "rb");
        FILE *output_file = fopen(output_filename, "wb");
        FILE *codes_file = fopen(codes_filename, "w");
        
        if (!input_file || !output_file || !codes_file) {
            printf("Error opening files\n");
            return 1;
        }

        printf("=== ENCODING ===\n");
        
        // 1. Подсчет частот
        unsigned int freq[256] = {0};
        countFrequencies(input_file, freq);
        
        // 2. Создание узлов
        int nodeCount;
        Node** nodes = createNodeArray(freq, &nodeCount);
        printf("Tree built. Unique symbols: %d\n", nodeCount);
        buildMinHeap(nodes, nodeCount);
        
        // 3. Построение дерева
        while (nodeCount > 1) {
            Node* left = extractMin(nodes, &nodeCount);
            Node* right = extractMin(nodes, &nodeCount);
            Node* parent = createInternalNode(left, right);

            nodes[nodeCount] = parent;
            nodeCount++;
            buildMinHeap(nodes, nodeCount);
        }
        
        Node* root = nodes[0];
        
        // 4. Генерация кодов
        char codes[256][256] = {0};
        char currentCode[256] = {0};
        generateCodes(root, currentCode, 0, codes);
        
        // 5. Сохранение кодов в файл
        for (int i = 0; i < 256; i++) {
            if (strlen(codes[i]) > 0) {
                fprintf(codes_file, "%d:%s\n", i, codes[i]);
            }
        }
        
        // 6. Кодирование с побитовой арифметикой
        encodeFileBit(input_file, output_file, codes);
        
        // Статистика
        fseek(input_file, 0, SEEK_END);//разместил в конце файла
        long inputSize = ftell(input_file);//вернул позицию каретки = размер файла
        fseek(output_file, 0, SEEK_END);
        long outputSize = ftell(output_file);//аналогично
        
        printf("Input size:  %ld bytes\n", inputSize);
        printf("Output size: %ld bytes\n", outputSize);
        printf("Compression: %.1f%%\n", (1.0 - (double)outputSize/inputSize) * 100);
        
        // Очистка
        fclose(input_file);
        fclose(output_file);
        fclose(codes_file);
        freeTree(root);
        free(nodes);
        
    } else if (strcmp(mode, "-d") == 0) {
        // ========== ДЕКОДИРОВАНИЕ ==========
        FILE *input_file = fopen(input_filename, "rb");
        FILE *output_file = fopen(output_filename, "wb");
        FILE *codes_file = fopen(codes_filename, "r");
        
        if (!input_file || !output_file || !codes_file) {
            printf("Error opening files\n");
            return 1;
        }
        
        printf("=== DECODING ===\n");
        
        // 1. Чтение кодов из файла
        char codes[256][256] = {0};
        char line[256];
        
        while (fgets(line, sizeof(line), codes_file)) {
            line[strcspn(line, "\n")] = 0;
            char* colon = strchr(line, ':');
            if (colon) {
                *colon = 0;
                int symbol = atoi(line);
                strcpy(codes[symbol], colon + 1);
            }
        }
        
        // 2. Восстановление дерева из кодов
        Node* root = createLeafNode(0, 0);
        
        for (int i = 0; i < 256; i++) {
            if (strlen(codes[i]) > 0) {
                Node* currentNode = root;
                for (int j = 0; codes[i][j] != '\0'; j++) {
                    if (codes[i][j] == '0') {
                        if (!currentNode->left) {
                            currentNode->left = createLeafNode(0, 0);
                        }
                        currentNode = currentNode->left;
                    } else {
                        if (!currentNode->right) {
                            currentNode->right = createLeafNode(0, 0);
                        }
                        currentNode = currentNode->right;
                    }
                }
                currentNode->symbol = i;
            }
        }
        
        // 3. Декодирование с побитовой арифметикой
        printf("Decoding with bit arithmetic...\n");
        //decodeFileBit(input_file, output_file, root);
        
        printf("Decoding complete!\n");
        
        // Очистка
        fclose(input_file);
        fclose(output_file);
        fclose(codes_file);
        freeTree(root);
        
    } else {
        printf("Unknown mode: %s\n", mode);
        return 1;
    }
    
    return 0;
}