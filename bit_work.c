#include "codeproc.h"
void encodeFileBit(FILE* input, FILE* output, char codes[256][256]) {
    unsigned char buffer = 0;  // Буфер для накопления битов
    int bitPos = 0;            // Текущая позиция в байте (0-7)
    
    rewind(input);
    int ch;
    
    // Сначала записываем количество полезных битов (будем записать в конце)
    long startPos = ftell(output);
    fwrite(&bitPos, sizeof(int), 1, output); // Заглушка, заполним позже
    
    while ((ch = fgetc(input)) != EOF) {
        const char* bits = codes[ch];
        
        for (int i = 0; bits[i] != '\0'; i++) {
            // Добавляем бит в буфер
            if (bits[i] == '1') {
                buffer |= (1 << (7 - bitPos));
            }
            
            bitPos++;
            
            // Если байт заполнился, записываем его
            if (bitPos == 8) {
                fputc(buffer, output);
                buffer = 0;
                bitPos = 0;
            }
        }
    }
    
    // Если остались биты в буфере, записываем последний байт
    if (bitPos > 0) {
        fputc(buffer, output);
    }
    
    // Записываем реальное количество битов в последнем байте
    long endPos = ftell(output);
    fseek(output, startPos, SEEK_SET);
    fwrite(&bitPos, sizeof(int), 1, output);
    fseek(output, endPos, SEEK_SET);
    
    printf("Encoded with bit arithmetic. Last byte has %d useful bits\n", bitPos);
}
