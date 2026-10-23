/*Упаковать массив
    В программе реализована структура данных:
    struct pack_array {
    uint32_t array; // поле для хранения упакованного массива из 0 и 1
    uint32_t count0 : 8; // счетчик нулей в array
    uint32_t count1 : 8; // счетчик единиц в array
    }
    Необходимо реализовать программу, которая упаковывает переданный ей массив из 32-ух элементов 0 и 1 в указанную структуру данных.
    Функция должна строго соответствовать прототипу:

    void array2struct(int [], struct pack_array *)*/

#include <stdio.h>
#include <stdint.h>

struct pack_array {
    uint32_t array;
    uint32_t count0 : 8;
    uint32_t count1 : 8;
};

void array2struct(int arr[], struct pack_array *packed) {
    packed->array = 0;
    packed->count0 = 0;
    packed->count1 = 0;

    for (int i = 0; i < 32; i++) {
        packed->array |= (uint32_t)arr[i] << (31 - i);

        if (arr[i] == 0) {
            packed->count0++;
        } else if (arr[i] == 1) {
            packed->count1++;
        }
    }
}

int main(void) {
    int arr[32];
    struct pack_array packed;

    for (int i = 0; i < 32; i++) {
        if (scanf("%d", &arr[i]) != 1 ||
            (arr[i] != 0 && arr[i] != 1)) {
            return 1;
        }
    }

    array2struct(arr, &packed);

    printf("%u %u %u\n",
           packed.array,
           packed.count0,
           packed.count1);

    return 0;
}