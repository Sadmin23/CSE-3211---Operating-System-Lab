#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include "generics.h"

void kprintf_basic(char *format, ...)
{
    char *tr;
    uint32_t i;
    uint8_t *str;
    va_list list;
    double dval;

    printf("basic\n");

    va_start(list, format);
    for (tr = format; *tr != '\0'; tr++)
    {
        while (*tr != '%' && *tr != '\0')
        {
            printf("%c", *tr);
            tr++;
        }
        if (*tr == '\0')
            break;
        tr++;
    }
    va_end(list);
}

void kprintf_driver(int driver, char *format, ...)
{
    char *tr;
    uint32_t i;
    uint8_t *str;
    va_list list;
    double dval;
    // uint32_t *intval;
    va_start(list, format);

    for (tr = format; *tr != '\0'; tr++)
    {
        while (*tr != '%' && *tr != '\0')
        {
            tr++;
        }
        if (*tr == '\0')
            break;
        tr++;
        switch (*tr)
        {

        case 'd':
            i = va_arg(list, int);
            if (i >= 0)
                printf("Printing LED no. %d", i);
            break;

        default:
            break;
        }
    }

    va_end(list);
}
int main(void)
{
    int x = 25;

    kprintf("Message\n");

    //    kprintf("%d", x); // Calls kprintf_basic

    kprintf(1, "%d", 42); // Calls kprintf_driver for driver 1

    return 0;
}
