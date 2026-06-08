#include <stdio.h>

int main(int argc, char **argv)
{
	int a;
    int even = 0;
    int odd = 0;

    scanf("%d", &a);

    if (a < 0)
    {
        a = -a;
    }

    while (a > 0)
    {
        int digit = a % 10;

        if (digit % 2 == 0)
        {
            even++;
        }
        else
        {
            odd++;
        }

        a /= 10;
    }

    printf("%d %d", even, odd);
	return 0;
}

