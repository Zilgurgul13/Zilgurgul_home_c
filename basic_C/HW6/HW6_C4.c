#include <stdio.h>

int f (int x)
{
	if (x >= -2 && x < 2)
	{
		return x * x;
	}
	else if (x >= 2)
	{
		return x * x + 4 * x + 5;
	}
	else 
	{
		return 4;
	}
} 

int main()
{
	int x;
	int max;
	int first = 1;
	
	while (scanf("%d", &x) == 1 && x != 0)
	{
		int y = f(x);
		
		if(first)
		{
			max = y;
			first = 0;
		}
		else if (y > max)
		{
			max = y;
		}
	}
		printf("%d", max);
		
	return 0;
}

