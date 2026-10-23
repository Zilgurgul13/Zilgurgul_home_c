/*Подсчет бит
    На вход программе подается беззнаковое 32-битное целое число N. 
    Требуется найти количество единичных битов в двоичном представлении данного числа.*/

#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint32_t N;
    unsigned int count = 0;

    scanf("%u", &N);

    while (N > 0) {
        count += N & 1u;
        N >>= 1;
    }

    printf("%u\n", count);

    return 0;
}