#include <stdio.h>

int abs(int a)
{
	if (a < 0)
	{
		return -a;
	}
	else
	{
		return a;
	}
}

int main(int argc, char **argv)
{
	int n;
	scanf("%d", &n);
	printf("%d", abs(n));
	return 0;
}

