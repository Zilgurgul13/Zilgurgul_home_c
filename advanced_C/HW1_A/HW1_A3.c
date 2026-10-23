/*Извлечение бит
    На вход программе подается беззнаковое 32-битное целое число N и натуральное число K (1 ≤ K ≤ 31). 
    Требуется взять K младших битов числа N и вывести полученное таким образом число.*/

#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint32_t N;
    unsigned int K;

    scanf("%u %u", &N, &K);

    uint32_t mask = (1u << K) - 1;
    uint32_t result = N & mask;

    printf("%u\n", result);

    return 0;
}