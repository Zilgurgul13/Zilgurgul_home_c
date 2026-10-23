#include <stdio.h>

int main(int argc, char **argv)
{
	int a;
    scanf("%d", &a);

    int digit = a % 10;
    int min = digit;
    int max = digit;

    while (a > 0)
    {
        digit = a % 10;

        if (digit < min)
        {
            min = digit;
        }

        if (digit > max)
        {
            max = digit;
        }

        a /= 10;
    }

    printf("%d %d", min, max);
	return 0;
}

