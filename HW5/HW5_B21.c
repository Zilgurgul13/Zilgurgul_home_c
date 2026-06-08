#include <stdio.h>

int main(int argc, char **argv)
{
	char c;
    while (1)
    {
        scanf("%c", &c);
        if (c == '.')
        {
            break;
        }
        if (c >= 'A' && c <= 'Z')
        {
            c = c - 'A' + 'a';
        }
        printf("%c", c);
    }
	return 0;
}

