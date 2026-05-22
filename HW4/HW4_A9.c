#include <stdio.h>

int main(int argc, char **argv)
{
	int a, b, c, d, e;
	int max;
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
	
	printf("%d", max);
		
	return 0;
}

