#include "codeproc.h"

void countFrequencies(FILE* file, unsigned int* freq){
    //для хранения аси-кода символа 
    int ch;

    //заполняю массив 256 нулями
    for (int i = 0; i <256; i++){
        freq[i] = 0;
    }

    rewind(file); //указатель в начало файла

    //прибаляю единичку к счетчику в ячейку соотвествующему аси-номеру буквы
    while ((ch = fgetc(file))!=EOF){
        freq[ch]++;
    }
}