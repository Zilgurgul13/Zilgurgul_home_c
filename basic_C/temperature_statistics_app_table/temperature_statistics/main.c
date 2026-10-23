#include "temp_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_month(const char *text)
{
    char *endptr;
    long value;

    if (text == NULL || *text == '\0') {
        return -1;
    }

    value = strtol(text, &endptr, 10);
    if (*endptr != '\0' || value < 1 || value > 12) {
        return -1;
    }

    return (int)value;
}

int main(int argc, char *argv[])
{
    const char *filename = NULL;
    int month = 0;
    int i;
    Temperature *records = NULL;
    size_t count = 0;

    if (argc == 1) {
        fprintf(stderr, "Ошибка: не указаны аргументы.\n");
        print_help(argv[0]);
        return 1;
    }

    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0) {
            print_help(argv[0]);
            return 0;
        }

        if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Ошибка: после -f требуется имя файла.\n");
                return 1;
            }
            filename = argv[++i];
            continue;
        }

        if (strcmp(argv[i], "-m") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Ошибка: после -m требуется номер месяца.\n");
                return 1;
            }
            month = parse_month(argv[++i]);
            if (month == -1) {
                fprintf(stderr, "Ошибка: номер месяца должен быть от 1 до 12.\n");
                return 1;
            }
            continue;
        }

        fprintf(stderr, "Ошибка: неизвестный ключ: %s\n", argv[i]);
        fprintf(stderr, "Используйте -h для справки.\n");
        return 1;
    }

    if (filename == NULL) {
        fprintf(stderr, "Ошибка: необходимо указать входной файл через -f.\n");
        return 1;
    }

    if (read_temperature_file(filename, &records, &count) != 0) {
        return 1;
    }

    if (count == 0) {
        fprintf(stderr, "Ошибка: файл не содержит корректных показаний.\n");
        free(records);
        return 1;
    }

    if (month != 0) {
        TemperatureStats stats;

        printf("Статистика за месяц %d\n", month);
        printf("+--------+------------------+------------------+------------------+------------------+\n");
        printf("| Месяц  | Средняя, °C      | Минимум, °C      | Максимум, °C     | Показаний        |\n");
        printf("+--------+------------------+------------------+------------------+------------------+\n");

        if (calculate_month_stats(records, count, month, &stats)) {
            printf("| %6d | %16.2f | %16d | %16d | %16zu |\n",
                   month, stats.average, stats.min, stats.max, stats.count);
        } else {
            printf("| %6d | %-16s | %16s | %16s | %16s |\n",
                   month, "нет данных", "—", "—", "0");
        }
        printf("+--------+------------------+------------------+------------------+------------------+\n");
    } else {
        TemperatureStats year_stats;

        printf("Статистика по месяцам\n");
        printf("+--------+------------------+------------------+------------------+------------------+\n");
        printf("| Месяц  | Средняя, °C      | Минимум, °C      | Максимум, °C     | Показаний        |\n");
        printf("+--------+------------------+------------------+------------------+------------------+\n");

        for (i = 1; i <= 12; ++i) {
            TemperatureStats month_stats;

            if (calculate_month_stats(records, count, i, &month_stats)) {
                printf("| %6d | %16.2f | %16d | %16d | %16zu |\n",
                       i, month_stats.average, month_stats.min,
                       month_stats.max, month_stats.count);
            } else {
                printf("| %6d | %-16s | %16s | %16s | %16s |\n",
                       i, "нет данных", "—", "—", "0");
            }
        }
        printf("+--------+------------------+------------------+------------------+------------------+\n");

        if (calculate_year_stats(records, count, &year_stats)) {
            printf("\nСтатистика за год\n");
            printf("+------------------+------------------+------------------+------------------+\n");
            printf("| Средняя, °C      | Минимум, °C      | Максимум, °C     | Показаний        |\n");
            printf("+------------------+------------------+------------------+------------------+\n");
            printf("| %16.2f | %16d | %16d | %16zu |\n",
                   year_stats.average, year_stats.min, year_stats.max,
                   year_stats.count);
            printf("+------------------+------------------+------------------+------------------+\n");
        }
    }

    free(records);
    return 0;
}
