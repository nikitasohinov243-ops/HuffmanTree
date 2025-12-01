#include "codeproc.h"

// Генерация кодов Хаффмана
void generateCodes(Node* root, char* currentCode, int depth, char codes[256][256]) {
    if (!root->left && !root->right) {
        // Это лист - сохраняем код для символа
        currentCode[depth] = '\0';  // Завершаем строку
        strcpy(codes[root->symbol], currentCode);
        return;
    }
    
    if (root->left != NULL) {
        currentCode[depth] = '0';  // Левый потомок = 0
        generateCodes(root->left, currentCode, depth + 1, codes);
    }
    
    if (root->right != NULL) {
        currentCode[depth] = '1';  // Правый потомок = 1  
        generateCodes(root->right, currentCode, depth + 1, codes);
    }
}


// Освобождение памяти дерева
void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Декодирование файла (заглушка - требует сохранения дерева)
void decodeFile(FILE* input, FILE* output, Node* root) {
    printf("Decoding requires saving the tree to a file!\n");
}

// Функция сравнения двух файлов
int compareFiles(FILE *file1, FILE *file2) {
    int char1, char2;
    int position = 0;
    
    rewind(file1);
    rewind(file2);
    
    while (1) {
        char1 = fgetc(file1);
        char2 = fgetc(file2);
        position++;
        
        if (char1 != char2) {
            printf("  Difference at position %d:\n", position);
            printf("    File 1: ");
            if (char1 == EOF) printf("EOF");
            else printf("'%c' (ASCII %d)", char1, char1);
            printf("\n    File 2: ");
            if (char2 == EOF) printf("EOF");
            else printf("'%c' (ASCII %d)", char2, char2);
            printf("\n");
            return 0; // Не совпадают
        }
        
        if (char1 == EOF) {
            break; // Оба файла закончились
        }
    }
    
    return 1; // Совпадают
}