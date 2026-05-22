#include <stdio.h>

int main(int argc, char **argv)
{
	int a, b, c, d, e;
	int max;
	int min;
	scanf("%d %d %d %d %d", &a, &b, &c, &d, &e);
	
	max = a;
	
	if (b > max)
	{
		max = b;
	}
	
	if (c > max)
	{
		max = c;
	}
	
	if (d > max)
	{
		max = d;
	}
	
	if (e > max)
	{
		max = e;
	}
	
	min = a;
	
	if (b < min)
	{
		min = b;
	}
	
	if (c < min)
	{
		min = c;
	}
	
	if (d < min)
	{
		min = d;
	}
	
	if (e < min)
	{
		min = e;
	}
	
	printf("%d", (max + min));
	
	return 0;
}

