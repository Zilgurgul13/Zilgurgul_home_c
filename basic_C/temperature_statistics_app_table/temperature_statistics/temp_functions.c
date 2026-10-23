#include "temp_functions.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 1024

static int is_leap_year(int year)
{
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

static int days_in_month(int year, int month)
{
    static const int days[] = {31, 28, 31, 30, 31, 30,
                               31, 31, 30, 31, 30, 31};

    if (month == 2 && is_leap_year(year)) {
        return 29;
    }
    return days[month - 1];
}

static char *trim_spaces(char *text)
{
    char *end;

    while (isspace((unsigned char)*text)) {
        text++;
    }

    end = text + strlen(text);
    while (end > text && isspace((unsigned char)end[-1])) {
        --end;
    }
    *end = '\0';
    return text;
}

static int parse_integer(const char *text, long *value)
{
    char *endptr;

    if (text == NULL || *text == '\0') {
        return 0;
    }

    *value = strtol(text, &endptr, 10);
    endptr = trim_spaces(endptr);
    return *endptr == '\0';
}

static int parse_record(char *line, Temperature *record)
{
    char *fields[6];
    char *token;
    long values[6];
    int field_count = 0;
    int i;

    token = strtok(line, ";");
    while (token != NULL && field_count < 6) {
        fields[field_count++] = trim_spaces(token);
        token = strtok(NULL, ";");
    }

    if (field_count != 6 || token != NULL) {
        return 0;
    }

    for (i = 0; i < 6; ++i) {
        if (!parse_integer(fields[i], &values[i])) {
            return 0;
        }
    }

    if (values[0] < 1000 || values[0] > 9999) {
        return 0;
    }
    if (values[1] < 1 || values[1] > 12) {
        return 0;
    }
    if (values[2] < 1 || values[2] > days_in_month((int)values[0], (int)values[1])) {
        return 0;
    }
    if (values[3] < 0 || values[3] > 23) {
        return 0;
    }
    if (values[4] < 0 || values[4] > 59) {
        return 0;
    }
    if (values[5] < -99 || values[5] > 99) {
        return 0;
    }

    record->year = (int)values[0];
    record->month = (int)values[1];
    record->day = (int)values[2];
    record->hour = (int)values[3];
    record->minute = (int)values[4];
    record->temperature = (int)values[5];

    return 1;
}

int read_temperature_file(const char *filename, Temperature **records, size_t *count)
{
    FILE *file;
    char line[512];
    size_t line_number = 0;
    size_t capacity = INITIAL_CAPACITY;
    Temperature *data;

    *records = NULL;
    *count = 0;

    file = fopen(filename, "r");
    if (file == NULL) {
        perror(filename);
        return -1;
    }

    data = malloc(capacity * sizeof(*data));
    if (data == NULL) {
        fclose(file);
        fprintf(stderr, "Ошибка: не удалось выделить память.\n");
        return -1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        Temperature record;
        char original_line[sizeof(line)];
        char *clean;
        size_t len;

        ++line_number;
        len = strlen(line);

        if (len > 0 && line[len - 1] != '\n' && !feof(file)) {
            int ch;
            while ((ch = fgetc(file)) != '\n' && ch != EOF) {
                /* discard the rest of the invalid line */
            }
            fprintf(stderr, "Ошибка в строке %zu: слишком длинная строка.\n", line_number);
            continue;
        }

        strncpy(original_line, line, sizeof(original_line) - 1);
        original_line[sizeof(original_line) - 1] = '\0';
        clean = trim_spaces(line);

        if (*clean == '\0') {
            fprintf(stderr, "Ошибка в строке %zu: пустая строка.\n", line_number);
            continue;
        }

        if (line_number == 1 &&
            (strcmp(clean, "YEAR;MONTH;DAY;HOUR;MINUTE;TEMPERATURE") == 0 ||
             strcmp(clean, "YEAR MONTH DAY HOUR MINUTE TEMPERATURE") == 0)) {
            continue;
        }

        if (!parse_record(clean, &record)) {
            fprintf(stderr, "Ошибка в строке %zu: неверный формат, строка пропущена: %s",
                    line_number, original_line);
            len = strlen(original_line);
            if (len == 0 || original_line[len - 1] != '\n') {
                fputc('\n', stderr);
            }
            continue;
        }

        if (*count >= capacity) {
            size_t new_capacity = capacity * 2;
            Temperature *new_data = realloc(data, new_capacity * sizeof(*data));
            if (new_data == NULL) {
                free(data);
                fclose(file);
                fprintf(stderr, "Ошибка: не удалось расширить память.\n");
                return -1;
            }
            data = new_data;
            capacity = new_capacity;
        }

        data[*count] = record;
        ++(*count);
    }

    if (ferror(file)) {
        perror("Ошибка чтения файла");
        free(data);
        fclose(file);
        return -1;
    }

    fclose(file);

    if (*count == 0) {
        free(data);
        data = NULL;
    }

    *records = data;
    return 0;
}

static int calculate_stats(const Temperature *records, size_t count, TemperatureStats *stats)
{
    size_t i;
    long long sum = 0;

    if (records == NULL || count == 0 || stats == NULL) {
        return 0;
    }

    stats->min = records[0].temperature;
    stats->max = records[0].temperature;

    for (i = 0; i < count; ++i) {
        int temperature = records[i].temperature;
        sum += temperature;

        if (temperature < stats->min) {
            stats->min = temperature;
        }
        if (temperature > stats->max) {
            stats->max = temperature;
        }
    }

    stats->count = count;
    stats->average = (double)sum / (double)count;
    return 1;
}

int calculate_month_stats(const Temperature *records, size_t count, int month, TemperatureStats *stats)
{
    size_t i;
    size_t month_count = 0;
    long long sum = 0;
    int min = 0;
    int max = 0;

    if (records == NULL || stats == NULL || month < 1 || month > 12) {
        return 0;
    }

    for (i = 0; i < count; ++i) {
        if (records[i].month == month) {
            int temperature = records[i].temperature;

            if (month_count == 0) {
                min = temperature;
                max = temperature;
            } else {
                if (temperature < min) {
                    min = temperature;
                }
                if (temperature > max) {
                    max = temperature;
                }
            }

            sum += temperature;
            ++month_count;
        }
    }

    if (month_count == 0) {
        return 0;
    }

    stats->average = (double)sum / (double)month_count;
    stats->min = min;
    stats->max = max;
    stats->count = month_count;
    return 1;
}

int calculate_year_stats(const Temperature *records, size_t count, TemperatureStats *stats)
{
    return calculate_stats(records, count, stats);
}

void print_stats(const char *title, const TemperatureStats *stats)
{
    printf("%s\n", title);
    printf("  Средняя температура: %.2f °C\n", stats->average);
    printf("  Минимальная температура: %d °C\n", stats->min);
    printf("  Максимальная температура: %d °C\n", stats->max);
    printf("  Количество показаний: %zu\n", stats->count);
}

void print_help(const char *program_name)
{
    printf("Статистика температуры за календарный год\n\n");
    printf("Использование:\n");
    printf("  %s -f <filename.csv>\n", program_name);
    printf("  %s -f <filename.csv> -m <номер месяца>\n", program_name);
    printf("  %s -h\n\n", program_name);

    printf("Ключи:\n");
    printf("  -h              описание функционала и список ключей\n");
    printf("  -f <filename>   входной CSV-файл\n");
    printf("  -m <номер>      вывести статистику только за указанный месяц (1-12)\n\n");

    printf("Формат строки CSV:\n");
    printf("  YEAR;MONTH;DAY;HOUR;MINUTE;TEMPERATURE\n");
    printf("  Пример: 2021;01;01;23;01;-5\n\n");

    printf("Некорректные строки пропускаются. Номер строки с ошибкой\n");
    printf("выводится в stderr.\n");
}
