#include <stdio.h>
#include <string.h>

int main(void)
{
    FILE *f;
		char str[1000], n;
		f = fopen("input.txt","r"); 
		fscanf (f, "%[^\n]", str);
        fclose (f);
		
		f = fopen("output.txt","w"); 
		n=strlen(str);
		for (int j=0; j<n-1; j++)
		{
			if(str[j]==str[n-1])				
				fprintf (f, "%d ",j);
		}
    fclose (f);
    return 0;
}
