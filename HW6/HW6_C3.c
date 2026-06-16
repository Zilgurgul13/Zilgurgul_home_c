#include <stdio.h>

int middle(int a, int b)
{
	return (a + b) / 2;
}

int main(int argc, char **argv)
{
	int a, b;
	scanf("%d %d", &a, &b);
	printf("%d", middle(a, b));
	return 0;
}

