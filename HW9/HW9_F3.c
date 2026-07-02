#include <stdio.h>

void count_digits(char num[])
{
    int count[10] = {0};
    int i;

    for (i = 0; num[i] != '\0'; i++)
    {
        count[num[i] - '0']++;
    }

    for (i = 0; i < 10; i++)
    {
        if (count[i] > 0)
        {
            printf("%d %d\n", i, count[i]);
        }
    }
}

int main()
{
    char num[1001];

    scanf("%1000s", num);

    count_digits(num);

    return 0;
}
