#include <stdio.h>

int main(int argc, char **argv)
{
	int a;
	scanf("%d", &a);
	if (a < 0)
	{
		a = -a;
	}
	while (a >= 10)
	{
		if (a % 10 == (a / 10) % 10)
		{
			printf("YES");
			return 0;
		}
		a = a / 10;
	}
	printf("NO");

	return 0;
}

