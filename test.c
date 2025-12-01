// #include "codeproc.h"
// #include "Min-Heap.h"
// #include <string.h>

// int main(int argc, char *argv[]) {
//     if (argc < 5) {
//         printf("Usage:\n");
//         printf("  Encode: %s -e input.txt output.huff codes.txt\n", argv[0]);
//         printf("  Decode: %s -d encoded.huff codes.txt output.txt\n", argv[0]);
//         return 1;
//     }

//     char* mode = argv[1];

//     if (strcmp(mode, "-e") == 0) {
//         // ========== КОДИРОВАНИЕ ==========
//         char* input_filename = argv[2];
//         char* encoded_filename = argv[3];
//         char* codes_filename = argv[4];
        
//         printf("=== ENCODING MODE ===\n");
//         printf("Input: %s\n", input_filename);
//         printf("Encoded output: %s\n", encoded_filename);
//         printf("Codes output: %s\n", codes_filename);
        
//         // 1. Открываем файлы
//         FILE *input_file = fopen(input_filename, "r");
//         if (!input_file) {
//             printf("ERROR: Cannot open input file\n");
//             return 1;
//         }
        
//         FILE *encoded_file = fopen(encoded_filename, "w");
//         FILE *codes_file = fopen(codes_filename, "w");
        
//         if (!encoded_file || !codes_file) {
//             printf("ERROR: Cannot open output files\n");
//             fclose(input_file);
//             return 1;
//         }
        
//         // 2. Проверяем размер файла
//         fseek(input_file, 0, SEEK_END);
//         long file_size = ftell(input_file);
//         rewind(input_file);
        
//         if (file_size == 0) {
//             printf("ERROR: Input file is empty!\n");
//             fclose(input_file);
//             fclose(encoded_file);
//             fclose(codes_file);
//             return 1;
//         }
        
//         printf("File size: %ld bytes\n", file_size);
        
//         // 3. Анализ частот
//         printf("\n1. Counting frequencies...\n");
//         unsigned int freq[256] = {0};
//         countFrequencies(input_file, freq);
        
//         // Выводим частоты
//         printf("Frequencies found:\n");
//         for (int i = 0; i < 256; i++) {
//             if (freq[i] > 0) {
//                 printf("  '%c' (ASCII %d): %d times\n", i, i, freq[i]);
//             }
//         }
        
//         // 4. Создаем узлы
//         printf("\n2. Creating nodes...\n");
//         int nodeCount = 0;
//         for (int i = 0; i < 256; i++) {
//             if (freq[i] > 0) nodeCount++;
//         }
//         printf("Unique symbols: %d\n", nodeCount);
        
//         if (nodeCount == 0) {
//             printf("ERROR: No symbols found!\n");
//             return 1;
//         }
        
//         // 5. Построение дерева (упрощенное)
//         printf("\n3. Building Huffman tree...\n");
        
//         // Создаем массив узлов вручную для теста
//         Node* nodes[256];
//         int count = 0;
//         for (int i = 0; i < 256; i++) {
//             if (freq[i] > 0) {
//                 nodes[count] = createLeafNode(i, freq[i]);
//                 count++;
//             }
//         }
        
//         // Сортируем по частоте (пузырьком)
//         for (int i = 0; i < count - 1; i++) {
//             for (int j = i + 1; j < count; j++) {
//                 if (nodes[j]->freq < nodes[i]->freq) {
//                     Node* temp = nodes[i];
//                     nodes[i] = nodes[j];
//                     nodes[j] = temp;
//                 }
//             }
//         }
        
//         // Строим дерево
//         while (count > 1) {
//             Node* left = nodes[0];
//             Node* right = nodes[1];
//             Node* parent = createInternalNode(left, right);
            
//             // Сдвигаем массив
//             nodes[0] = parent;
//             for (int i = 1; i < count - 1; i++) {
//                 nodes[i] = nodes[i + 1];
//             }
//             count--;
            
//             // Сортируем снова
//             for (int i = 0; i < count - 1; i++) {
//                 for (int j = i + 1; j < count; j++) {
//                     if (nodes[j]->freq < nodes[i]->freq) {
//                         Node* temp = nodes[i];
//                         nodes[i] = nodes[j];
//                         nodes[j] = temp;
//                     }
//                 }
//             }
//         }
        
//         Node* root = nodes[0];
//         printf("Tree built. Root frequency: %d\n", root->freq);
        
//         // 6. Генерация кодов
//         printf("\n4. Generating codes...\n");
//         char codes[256][256] = {0};
//         char currentCode[256] = {0};
//         generateCodes(root, currentCode, 0, codes);
        
//         // Проверяем коды
//         printf("Generated codes:\n");
//         int codes_found = 0;
//         for (int i = 0; i < 256; i++) {
//             if (strlen(codes[i]) > 0) {
//                 printf("  '%c' (ASCII %d): %s\n", i, i, codes[i]);
//                 codes_found++;
//             }
//         }
        
//         if (codes_found == 0) {
//             printf("ERROR: No codes generated! Using test codes...\n");
//             // Тестовые коды для AAABBCCCCCCC
//             strcpy(codes['A'], "01");
//             strcpy(codes['B'], "00");
//             strcpy(codes['C'], "1");
//         }
        
//         // 7. Сохраняем коды в файл
//         printf("\n5. Saving codes to %s...\n", codes_filename);
//         int saved_codes = 0;
//         for (int i = 0; i < 256; i++) {
//             if (strlen(codes[i]) > 0) {
//                 fprintf(codes_file, "%d:%s\n", i, codes[i]);
//                 saved_codes++;
//                 printf("  Written: %d:%s\n", i, codes[i]);
//             }
//         }
//         fclose(codes_file);
//         printf("Total codes saved: %d\n", saved_codes);
        
//         // 8. Кодируем данные
//         printf("\n6. Encoding data to %s...\n", encoded_filename);
//         rewind(input_file);
//         int ch;
//         long bits_written = 0;
//         while ((ch = fgetc(input_file)) != EOF) {
//             if (ch >= 0 && ch <= 255 && strlen(codes[ch]) > 0) {
//                 fprintf(encoded_file, "%s", codes[ch]);
//                 bits_written += strlen(codes[ch]);
//             }
//         }
//         fclose(encoded_file);
        
//         printf("Total bits written: %ld\n", bits_written);
//         printf("\n=== ENCODING COMPLETE ===\n");
        
//         fclose(input_file);
//         freeTree(root);
        
//     } else if (strcmp(mode, "-d") == 0) {
//         // ========== ДЕКОДИРОВАНИЕ ==========
//         printf("Decoding mode not implemented in debug version\n");
//         return 1;
        
//     } else {
//         printf("Unknown mode: %s\n", mode);
//         return 1;
//     }
    
//     return 0;
// }
