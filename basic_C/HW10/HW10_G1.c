#include <stdio.h>
#include <string.h>

int main()
{
    FILE *fin, *fout;
    char str[101];
    int len;

    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

    fgets(str, sizeof(str), fin);

    len = strlen(str);

    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
        len--;
    }

    fprintf(fout, "%s, %s, %s %d", str, str, str, len);

    fclose(fin);
    fclose(fout);

    return 0;
}
