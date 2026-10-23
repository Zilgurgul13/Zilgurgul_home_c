#include <stdio.h>

int main(int argc, char **argv)
{
	int a = 0, b = 0, c = 0;
	scanf("%d %d %d", &a, &b, &c);
	printf("%.2f\n", (float)(a + b + c) / 3);
	return 0;
}

