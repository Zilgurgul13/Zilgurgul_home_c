#include <stdio.h>

int main(int argc, char **argv)
{
	int number = 0, number100 = 0, number10 = 0, number1 = 0;
	scanf("%d", &number);
	number100 = number / 100;
	number10 = (number / 10) % 10;
	number1 = number % 10;
	printf("%d\n", number100 + number10 + number1);
	return 0;
}

