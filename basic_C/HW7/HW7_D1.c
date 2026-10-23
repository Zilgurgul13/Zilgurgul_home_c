#include <stdio.h>

void rec(int N)
{
	if (N == 0)
	{
		return;
	}
	rec(N - 1);
	printf("%d ", N);
} 


int main()
{
	int N;
	scanf("%d", &N);
	rec(N);
	return 0;
}

