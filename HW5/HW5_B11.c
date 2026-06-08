#include <stdio.h>

int main(int argc, char **argv)
{
	int a, reverse = 0;

    scanf("%d", &a);

    while (a != 0)
    {
        reverse = reverse * 10 + a % 10;
        a /= 10;
    }

    printf("%d", reverse);
	return 0;
}

