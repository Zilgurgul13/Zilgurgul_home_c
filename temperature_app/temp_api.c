#include <stdio.h>
#include "temp_api.h"

double average_temperature(const Temperature data[], int count)
{
    int sum = 0;

    for (int i = 0; i < count; i++)
        sum += data[i].temperature;

    return (double)sum / count;
}

int min_temperature(const Temperature data[], int count)
{
    int min = data[0].temperature;

    for (int i = 1; i < count; i++)
        if (data[i].temperature < min)
            min = data[i].temperature;

    return min;
}

int max_temperature(const Temperature data[], int count)
{
    int max = data[0].temperature;

    for (int i = 1; i < count; i++)
        if (data[i].temperature > max)
            max = data[i].temperature;

    return max;
}

void print_month_stats(const Temperature data[], int count,
                       int year, int month)
{
    int first = 1;
    int sum = 0, min = 0, max = 0, n = 0;

    for (int i = 0; i < count; i++) {
        if (data[i].year == year && data[i].month == month) {
            int t = data[i].temperature;

            sum += t;

            if (first) {
                min = t;
                max = t;
                first = 0;
            } else {
                if (t < min) min = t;
                if (t > max) max = t;
            }

            n++;
        }
    }

    if (n == 0) {
        printf("No data for %04d-%02d.\n", year, month);
        return;
    }

    printf("Average temperature: %.2f\n", (double)sum / n);
    printf("Minimum temperature: %d\n", min);
    printf("Maximum temperature: %d\n", max);
}

void print_year_stats(const Temperature data[], int count, int year)
{
    int first = 1;
    int sum = 0, min = 0, max = 0, n = 0;

    for (int i = 0; i < count; i++) {
        if (data[i].year == year) {
            int t = data[i].temperature;

            sum += t;

            if (first) {
                min = t;
                max = t;
                first = 0;
            } else {
                if (t < min) min = t;
                if (t > max) max = t;
            }

            n++;
        }
    }

    if (n == 0) {
        printf("No data for %d.\n", year);
        return;
    }

    printf("Average temperature: %.2f\n", (double)sum / n);
    printf("Minimum temperature: %d\n", min);
    printf("Maximum temperature: %d\n", max);
}
