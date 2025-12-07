#include "codeproc.h"
#include <string.h>

// /Users/nikitasohinov/Documents/ВШЭ/Лабы/programirovanie.2/labs/HuffmanTree
// ./huffman -e input.txt output.bin codes.txt
// ./huffman -d output.bin output_decode.txt codes.txt
// gcc -o huffman tree.c node_leaf.c min-heap.c freq.c codes.c bit_work.c rebuild_tree.c

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

        fprintf(codes_file, "FREQUENCIES:\n");
        for (int i = 0; i < 256; i++) {
            if (freq[i] > 0) {
                if (i >= 32 && i <= 126) { // Печатные символы
                    fprintf(codes_file, "%c: %u\n", (char)i, freq[i]);
                } else { // Непечатные символы
                    fprintf(codes_file, "0x%02X: %u\n", i, freq[i]);
                }
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

        unsigned int freq[256] = {0};
        
        // Читаем файл построчно
        char line[100];
        
        // Пропускаем строку "FREQUENCIES:"
        fgets(line, sizeof(line), codes_file);
        
        // Читаем частоты пока не встретим пустую строку
        while (fgets(line, sizeof(line), codes_file)) {
            // Убираем \n
            line[strcspn(line, "\n")] = '\0';
            
            // Если пустая строка - заканчиваем
            if (strlen(line) == 0) {
                break;
            }
            
            // Парсим: "символ: частота"
            char symbol_part[10];
            unsigned int frequency;
            
            if (sscanf(line, "%s %u", symbol_part, &frequency) == 2) {
                // symbol_part будет "X:" (например "A:"), убираем двоеточие
                char symbol = symbol_part[0];
                freq[(int)symbol] = frequency;
            }
        }
        
        Node* root = rebuildTreeFromFreq(freq);
        
        // Декодирование
        fseek(input_file, 0, SEEK_SET);
        decodeFileBit(input_file, output_file, root);
        
        printf("Decoding complete!\n");
        
        fclose(input_file);
        fclose(output_file);
        fclose(codes_file);
        freeTree(root);
        
    } else {
        printf("Unknown mode: %s\n", mode);
        return 1;
    }
}