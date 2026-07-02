#include <stdio.h>
#include <stdlib.h>

void sort_even_odd(int n, int a[]) 
{
    int *temp = (int *)malloc(n * sizeof(int));
    if (!temp) return;

    int idx = 0;

    for (int i = 0; i < n; i++) 
    {
        if (a[i] % 2 == 0) 
        {
            temp[idx++] = a[i];
        }
    }

    for (int i = 0; i < n; i++) 
    {
        if (a[i] % 2 != 0) 
        {
            temp[idx++] = a[i];
        }
    }

    for (int i = 0; i < n; i++) 
    {
        a[i] = temp[i];
    }

    free(temp);
}
