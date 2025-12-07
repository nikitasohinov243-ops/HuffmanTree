// #include "codeproc.h"
// #include <string.h>

// int main(int argc, char *argv[]) {
//     if (argc < 5) {
//         printf("Wrong arguments! Usage:\n");
//         printf("  Encode: %s -e input.txt output.huff codes.txt\n", argv[0]);
//         printf("  Decode: %s -d input.huff output.txt codes.txt\n", argv[0]);
//         return 1;
//     }

//     char* mode = argv[1];
//     char* input_filename = argv[2];
//     char* output_filename = argv[3];
//     char* codes_filename = argv[4];

//     if (strcmp(mode, "-e") == 0) {
//         // РЕЖИМ КОДИРОВАНИЯ
//         FILE *input_file = fopen(input_filename, "r");
//         FILE *output_file = fopen(output_filename, "w");  // Только 0 и 1
//         FILE *codes_file = fopen(codes_filename, "w");      // Таблица кодов
        
//         if (!input_file) {
//             printf("Error: Cannot open input file: %s\n", input_filename);
//             return 1;
//         }
//         if (!output_file) {
//             printf("Error: Cannot open output file: %s\n", output_filename);
//             fclose(input_file);
//             return 1;
//         }
//         if (!codes_file) {
//             printf("Error: Cannot open codes file: %s\n", codes_filename);
//             fclose(input_file);
//             fclose(output_file);
//             return 1;
//         }

//         // Подсчет частот
//         unsigned int freq[256];
//         countFrequencies(input_file, freq);

//         // Построение дерева Хаффмана
//         int nodeCount;
//         Node** nodes = createNodeArray(freq, &nodeCount);
//         buildMinHeap(nodes, nodeCount);

//         while (nodeCount > 1) {
//             Node* left = extractMin(nodes, &nodeCount);
//             Node* right = extractMin(nodes, &nodeCount);
//             Node* parent = createInternalNode(left, right);
            
//             // Вставка нового узла
//             nodes[nodeCount] = parent;
//             nodeCount++;
//             buildMinHeap(nodes, nodeCount);
//         }

//         Node* root = nodes[0];
//         printf("Tree built! Root frequency: %d\n", root->freq);
        
//         // ГЕНЕРИРУЮ КОД
//         char codes[256][256] = {0};
//         char currentCode[256] = {0};
//         generateCodes(root, currentCode, 0, codes);

//         // Вывод сгенерированных кодов для отладки
//         printf("\n=== Generated Codes ===\n");
//         for (int i = 0; i < 256; i++) {
//             if (strlen(codes[i]) > 0) {
//                 if (i >= 32 && i <= 126) { // Печатаю символы
//                     printf("'%c' (ASCII %3d): %s\n", i, i, codes[i]);
//                 } else {
//                     printf("ASCII %3d: %s\n", i, codes[i]);
//                 }
//             }
//         }
//         printf("\n");

//         printf("Saving codes to: %s\n", codes_filename);
//         for (int i = 0; i < 256; i++) {
//             if (strlen(codes[i]) > 0) {
//                 fprintf(codes_file, "%c:%s\n", i, codes[i]);
//             }
//         }

//         // Кодирование данных
//         rewind(input_file);
//         int ch;
//         while ((ch = fgetc(input_file)) != EOF) {
//             if (ch >= 0 && ch <= 255) {
//                 // Просто записываем код как текст
//                 fprintf(output_file, "%s", codes[ch]);
//             }
//         }

//         // Статистика
//         printf("The file is encrypted!\n");
//         printf("Source file: %s\n", input_filename);
//         printf("Encoded file: %s\n", output_filename);

//         // Закрытие файлов и очистка
//         fclose(input_file);
//         fclose(output_file);
//         fclose(codes_file);
//         freeTree(root);
//         free(nodes);

//     } else if (strcmp(mode, "-d") == 0) {
//         // РЕЖИМ ДЕКОДИРОВАНИЯ
//         char* encoded_filename = argv[2];  // "output.huff" (0 и 1)
//         char* codes_filename = argv[3];    // "codes.txt" (таблица кодов)
//         char* output_filename = argv[4];   // "decoded.txt" (результат)

//         FILE *encoded_file = fopen(encoded_filename, "r");  // 0 и 1
//         FILE *codes_file = fopen(codes_filename, "r");      // таблица кодов
//         FILE *output_file = fopen(output_filename, "w");

//         if (!encoded_file) {
//             printf("Error: Cannot open encoded file: %s\n", encoded_filename);
//             return 1;
//         }
//         if (!codes_file) {
//             printf("Error: Cannot open codes file: %s\n", codes_filename);
//             fclose(encoded_file);
//             return 1;
//         }
//         if (!output_file) {
//             printf("Error: Cannot open output file: %s\n", output_filename);
//             fclose(encoded_file);
//             fclose(codes_file);
//             return 1;
//         }

//         //Читаю таблицу кодов
//         char codes[256][256] = {0};
//         char line[256];
        
//         while (fgets(line, sizeof(line), codes_file)) {
//             line[strcspn(line, "\r\n")] = '\0';
            
//             char* colon = strchr(line, ':');
//             if (colon) {
//                 *colon = '\0';
//                 char* code_str = colon + 1;
                
//                 int symbol = atoi(line);
//                 strcpy(codes[symbol], code_str);
//             }
//         }
//         fclose(codes_file);

//         printf("Decoding data...\n");
//         char buffer[256] = {0};
//         int buf_pos = 0;
//         int bit;
        
//         while ((bit = fgetc(encoded_file)) != EOF) {
//             if (bit == '0' || bit == '1') {
//                 buffer[buf_pos++] = (char)bit;
//                 buffer[buf_pos] = '\0';
                
//                 // Ищем этот код в таблице
//                 for (int i = 0; i < 256; i++) {
//                     if (strlen(codes[i]) > 0 && strcmp(buffer, codes[i]) == 0) {
//                         fputc(i, output_file);  // Записываем символ
//                         buf_pos = 0;
//                         buffer[0] = '\0';
//                         break;
//                     }
//                 }
                
//                 // Защита от ошибок
//                 if (buf_pos >= 255) {
//                     printf("Error: Code too long or corrupted data!\n");
//                     break;
//                 }
//             }
//         }
        
//         printf("\nDecoding complete!\n");
//         printf("Output file: %s\n", output_filename);
//         printf("\n=== VERIFICATION ===\n");

//         // Переоткрываем output_file для чтения
//         fclose(output_file);
//         output_file = fopen(output_filename, "r");

//         // Открываем оригинальный файл если он был передан
//         if (argc >= 6) {
//             char* original_filename = argv[5];
//             FILE *original_file = fopen(original_filename, "r");
            
//             if (original_file && output_file) {
//                 if (compareFiles(original_file, output_file)) {
//                     printf("SUCCESS: Decoded file matches original!\n");
//                 } else {
//                     printf("FAILURE: Decoded file differs from original!\n");
//                 }
//                 fclose(original_file);
//             }
//         }

        
//         fclose(encoded_file);
//         fclose(output_file);
//     } else {
//         printf("Unknown mode: %s\n", mode);
//         printf("Use -e for encode or -d for decode\n");
//         return 1;
//     }
    
//     return 0;
// }