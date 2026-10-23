#include <stdio.h>

int main(int argc, char **argv)
{
	int a;
	scanf("%d", &a);
	if (a >= 100 && a <= 999) 
	{
		printf("YES");
	}
	else 
	{
		printf("NO");
	}
	
	return 0;
}

