#include <stdio.h>

int main(int argc, char **argv)
{
	int a;
    int count = 0;

    scanf("%d", &a);

    if (a < 0)
    {
        a = -a;
    }

    while (a > 0)
    {
        if (a % 10 == 9)
        {
            count++;
        }

        a /= 10;
    }

    if (count == 1)
    {
        printf("YES");
    }
    else
    {
        printf("NO");
    }
	return 0;
}

