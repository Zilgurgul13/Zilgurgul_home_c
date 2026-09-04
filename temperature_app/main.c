#include <stdio.h>
#include "temp_api.h"

#define MAX_RECORDS 1000

int main(void)
{
    Temperature data[MAX_RECORDS];
    int count, choice;

    printf("Temperature statistics\n");
    printf("Enter number of records (1-%d): ", MAX_RECORDS);
    scanf("%d", &count);

    if (count < 1 || count > MAX_RECORDS) {
        printf("Invalid number of records.\n");
        return 1;
    }

    printf("Enter records: year month day hour minute temperature\n");

    for (int i = 0; i < count; i++) {
        scanf("%d %d %d %d %d %d",
              &data[i].year, &data[i].month, &data[i].day,
              &data[i].hour, &data[i].minute, &data[i].temperature);
    }

    do {
        printf("\n1 - Monthly statistics\n");
        printf("2 - Year statistics\n");
        printf("0 - Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int year, month;
            printf("Enter year and month: ");
            scanf("%d %d", &year, &month);
            print_month_stats(data, count, year, month);
        } else if (choice == 2) {
            int year;
            printf("Enter year: ");
            scanf("%d", &year);
            print_year_stats(data, count, year);
        }
    } while (choice != 0);

    return 0;
}
