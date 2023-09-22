#ifndef GENERICS_H
#define GENERICS_H

void kprintf_basic(char *format, ...);
void kprintf_driver(int driver, char *format, ...);

#define kprintf(...) _Generic((__VA_ARGS__), \
    char *: kprintf_basic,                   \
    int: kprintf_driver)(__VA_ARGS__)

#endif
