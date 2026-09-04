#ifndef TEMP_API_H
#define TEMP_API_H

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int temperature;
} Temperature;

double average_temperature(const Temperature data[], int count);
int min_temperature(const Temperature data[], int count);
int max_temperature(const Temperature data[], int count);

void print_month_stats(const Temperature data[], int count,
                       int year, int month);
void print_year_stats(const Temperature data[], int count, int year);

#endif
