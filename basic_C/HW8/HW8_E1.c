#include <stdio.h>

int main()
{
	int a[5];
	double sum = 0;
	double average;
	
	for (int i = 0; i < 5; i++)
	{
		scanf("%d", &a[i]);
		sum += a[i];
	}
	
	average = sum / 5;
	
	printf("%3f\n", average);
	
	return 0;
}

