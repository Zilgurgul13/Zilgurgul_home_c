#include <stdio.h>

int main(int argc, char **argv)
{
	int a;
    scanf("%d", &a);

    if (a < 0)
    {
        a = -a;
    }

    while (a > 0)
    {
        int digit = a % 10;
        int temp = a / 10;

        while (temp > 0)
        {
            if (digit == temp % 10)
            {
                printf("YES");
                return 0;
            }

            temp /= 10;
        }

        a /= 10;
    }

    printf("NO");
	return 0;
}

