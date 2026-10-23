/*Распаковать порядок
    Необходимо реализовать программу, которой на входе передаётся вещественное число в типе float, 
    она возвращает порядок который хранится в поле EXP в виде десятичного целого числа. 
    Функция должна строго соответствовать прототипу*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

int extractExp(float x) {
    uint32_t bits;

    memcpy(&bits, &x, sizeof(bits));

    return (int)((bits >> 23) & 0xFF);
}

int main(void) {
    float x;

    if (scanf("%f", &x) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    printf("%d\n", extractExp(x));

    return 0;
}