#include <stdio.h>

int main()
{
	int a[10];
	int min_val, min_idx;
	int max_val, max_idx;
	
	for (int i = 0; i < 10; i++)
	{
		scanf("%d", &a[i]);
	}
	
	min_val = a[0];
	max_val = a[0];
	min_idx = 0;
	max_idx = 0;
	
	for (int i = 1; i < 10; i++)
	{
		if (a[i] < min_val)
		{
			min_val = a[i];
			min_idx = i;
		}
		if (a[i] > max_val)
		{
			max_val = a[i];
			max_idx = i;
		}
	}
	
	printf("%d %d %d %d", max_idx + 1, max_val, min_idx + 1, min_val);
		
	return 0;
}

