#ifndef TEMP_FUNCTIONS_H
#define TEMP_FUNCTIONS_H

#include <stddef.h>

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int temperature;
} Temperature;

typedef struct {
    double average;
    int min;
    int max;
    size_t count;
} TemperatureStats;

int read_temperature_file(const char *filename, Temperature **records, size_t *count);
int calculate_month_stats(const Temperature *records, size_t count, int month, TemperatureStats *stats);
int calculate_year_stats(const Temperature *records, size_t count, TemperatureStats *stats);
void print_stats(const char *title, const TemperatureStats *stats);
void print_help(const char *program_name);

#endif
