#include <stdio.h>

int main(int argc, char **argv)
{
	int number;
	int max;
	int number100, number10, number1;
	scanf("%d", &number);
	number100 = number / 100;
	number10 = (number / 10) % 10;
	number1 = number % 10;
	max = number100;
	if (number10 > max)
	{
		max = number10;
	}
	if (number1 > max)
	{
		max = number1;
	}
	printf("%d", max); 
	return 0;
}

