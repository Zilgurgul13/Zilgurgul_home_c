#include <stdio.h>

int main()
{
    FILE *fin, *fout;
    int N, i;
    char letter = 'A';
    char digit = '2';

    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

    fscanf(fin, "%d", &N);

    for (i = 1; i <= N; i++)
    {
        if (i % 2 == 1)
        {
            fprintf(fout, "%c", letter);
            letter++;
        }
        else
        {
            fprintf(fout, "%c", digit);
            digit += 2;

            if (digit > '8')
            {
                digit = '2';
            }
        }
    }

    fclose(fin);
    fclose(fout);

    return 0;
}

