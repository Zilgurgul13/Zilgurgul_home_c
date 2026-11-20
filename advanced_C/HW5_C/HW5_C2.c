/*Польская запись
Необходимо вычислить выражение написанное в обратной польской записи. 
На вход подается строка состоящая из целых, неотрицательных чисел и арифметических символов. В ответ единственное целое число - результат.*/




#include <stdio.h>
#include <ctype.h>

int main(void)
{
    char str[10001];
    int stack[10000];
    int top = -1;

    fgets(str, sizeof(str), stdin);

    int i = 0;

    while (str[i] != '.' && str[i] != '\0') {

        // Пропускаем пробелы
        if (isspace((unsigned char)str[i])) {
            i++;
            continue;
        }

        // Считываем число
        if (isdigit((unsigned char)str[i])) {
            int num = 0;

            while (isdigit((unsigned char)str[i])) {
                num = num * 10 + (str[i] - '0');
                i++;
            }

            stack[++top] = num;
        }
        else {
            int b = stack[top--];
            int a = stack[top--];

            int result = 0;

            switch (str[i]) {
                case '+':
                    result = a + b;
                    break;

                case '-':
                    result = a - b;
                    break;

                case '*':
                    result = a * b;
                    break;

                case '/':
                    result = a / b;
                    break;
            }

            stack[++top] = result;
            i++;
        }
    }

    printf("%d\n", stack[top]);

    return 0;
}

