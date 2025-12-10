#include "codeproc.h"

void encodeFileBit(FILE* input, FILE* output, char codes[256][256]) {
    // Сначала подсчитаем totalBits
    rewind(input);
    long totalBits = 0;
    int ch;
    while ((ch = fgetc(input)) != EOF) {
        totalBits += strlen(codes[ch]);
    }

    // Записываем totalBits в начало (8 байт)
    fwrite(&totalBits, sizeof(long), 1, output);

    // Теперь кодируем
    rewind(input);
    unsigned char buffer = 0;
    int bitPos = 0;

    while ((ch = fgetc(input)) != EOF) {
        const char* bits = codes[ch];
        for (int i = 0; bits[i]; i++) {
            if (bits[i] == '1') {
                buffer |= (1 << (7 - bitPos));
            }
            bitPos++;
            if (bitPos == 8) {
                fputc(buffer, output);
                buffer = 0;
                bitPos = 0;
            }
        }
    }

    if (bitPos > 0) {
        fputc(buffer, output);
    }

    printf("Encoded. Total bits: %ld\n", totalBits);
}

void decodeFileBit(FILE* input, FILE* output, Node* root) {
    long totalBits;
    fread(&totalBits, sizeof(long), 1, input);

    if (totalBits == 0) return;

    Node* current = root;//позиция в дереве. Начну с корня
    unsigned char byte;//буффер для чтения одного байта с файла
    long bitsProcessed = 0;

    while (bitsProcessed < totalBits && fread(&byte, 1, 1, input) == 1) { //пока проверенных битов меньше общего количество битов И можно прочесть один лемент размером один байт
        for (int i = 7; i >= 0 && bitsProcessed < totalBits; i--) { //читаем биты
            int bit = (byte >> i) & 1;//оставляем только последний бит
            bitsProcessed++;

            if (bit == 1) {
                current = current->right;
            } else {
                current = current->left;
            }

            if (!current->left && !current->right) {
                fputc(current->symbol, output);
                current = root;
            }
        }
    }

    printf("Decoded %ld bits\n", bitsProcessed);
}