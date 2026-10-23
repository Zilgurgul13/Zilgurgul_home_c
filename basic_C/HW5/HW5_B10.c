#include <stdio.h>

int main(int argc, char **argv)
{
	int a;
    scanf("%d", &a);

    if (a < 0)
    {
        a = -a;
    }

    int last = a % 10;
    a /= 10;

    while (a > 0)
    {
        int digit = a % 10;

        if (digit >= last)
        {
            printf("NO");
            return 0;
        }

        last = digit;
        a /= 10;
    }

    printf("YES");
	return 0;
}

