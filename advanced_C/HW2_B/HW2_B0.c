/*  Максимальный блок
Описана структура данных
typedef struct list {
uint64_t address;
size_t size;
char comment[64];
struct list *next;
} list;
Требуется реализовать только одну функцию, которая в данном списке находит адрес блока памяти занимающий больше всего места.
Адрес хранится в поле address, поле size - соответствующий размер данного блока. Если список пустой, то функция должна возвращать 0. Если есть несколько таких блоков, то вернуть адрес любого из них.
Прототип функции:
uint64_t findMaxBlock(list *head)*/

uint64_t findMaxBlock(list *head) {
    if (head == NULL) {
        return 0;
    }

    uint64_t maxAddress = head->address;
    size_t maxSize = head->size;

    list *current = head->next;

    while (current != NULL) {
        if (current->size > maxSize) {
            maxSize = current->size;
            maxAddress = current->address;
        }

        current = current->next;
    }

    return maxAddress;
}