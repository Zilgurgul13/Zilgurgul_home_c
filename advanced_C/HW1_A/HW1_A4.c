/*Поиск последовательности бит
    На вход программе подается беззнаковое 32-битное целое число N и натуральное число K (1 ≤ K ≤ 31). 
    Требуется взять K подряд идущих битов числа N так, чтобы полученное число было максимальным. 
    Программа должна вывести полученное число.*/

#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint32_t N;
    unsigned int K;

    scanf("%u %u", &N, &K);

    uint32_t mask = (1u << K) - 1;
    uint32_t max = 0;

    for (unsigned int i = 0; i <= 32 - K; i++) {
        uint32_t current = (N >> i) & mask;

        if (current > max) {
            max = current;
        }
    }

    printf("%u\n", max);

    return 0;
}
