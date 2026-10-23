#include <stdio.h>

int main(int argc, char **argv)
{
	int a;
    scanf("%d", &a);

    for (int i = 10; i <= a; i++)
    {
        int temp = i;
        int sum = 0;
        int product = 1;

        while (temp > 0)
        {
            int digit = temp % 10;
            sum += digit;
            product *= digit;
            temp /= 10;
        }

        if (sum == product)
        {
            printf("%d ", i);
        }
    }
	return 0;
}

