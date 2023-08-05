#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

void kprintf(char *format, ...)
{
    char *tr;
    uint32_t i;
    uint8_t *str;
    va_list list;

    char abc[50] = "";
    char str1[] = "LCD";

    int j = 0;

    va_start(list, format);

    for (tr = format; *tr != '\0'; tr++)
    {
        while (*tr != '%' && *tr != '\0')
        {
            abc[j++] = *tr;
            tr++;
        }
        if (strcmp(abc, str1) != 0)
        {
            printf("%s", abc);
            strcpy(abc, " ");
        }
        if (*tr == '\0')
        {
            break;
        }
        tr++;
        switch (*tr)
        {

        case 'd':
            i = va_arg(list, int);
            if (i >= 0)
                printf("%d", i);
            break;

        default:
            break;
        }
    }

    if (strcmp(abc, str1) == 0)
    {
        int a = va_arg(list, int);
        printf("LED %d\n", a);
    }

    va_end(list);
}

int main(void)
{
    kprintf("HELLO %d %d", 25, 3);
    // kprintf("Hello");

    return 0;
}
