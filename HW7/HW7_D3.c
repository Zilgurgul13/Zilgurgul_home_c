#include <stdio.h>


void rec(int N)
{
    printf("%d ", N % 10);

    if (N >= 10)
    {
        rec(N / 10);
    }
}

int main()
{
    int N;

    scanf("%d", &N);

    rec(N);

    return 0;
}

