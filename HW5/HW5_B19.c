#include <stdio.h>

int main(int argc, char **argv)
{
	int a;
    int sum = 0;

    scanf("%d", &a);

    while (a > 0)
    {
        sum += a % 10;
        a /= 10;
    }

    if (sum == 10)
    {
        printf("YES");
    }
    else
    {
        printf("NO");
    }
	return 0;
}

