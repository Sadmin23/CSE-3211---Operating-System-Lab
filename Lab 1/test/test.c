#include <stdio.h>
#include <stdlib.h>

int led_sets[10][10] = {
    {0, 1, 4, 5, 6, -1, 8, 9, 10, 11},      // 0
    {-1, -1, -1, 5, -1, -1, -1, 9, -1, -1}, // 1
    {0, 1, -1, 5, 6, 7, 8, -1, 10, 11},     // 2
    {0, 1, -1, 5, 6, 7, -1, 9, 10, 11},     // 3
    {0, -1, 4, 5, 6, 7, -1, 9, -1, -1},     // 4
    {0, 1, 4, -1, 6, 7, -1, 9, 10, 11},     // 5
    {0, 1, 4, -1, 6, 7, 8, 9, 10, 11},      // 6
    {0, 1, -1, 5, -1, -1, -1, 9, -1, -1},   // 7
    {0, 1, 4, 5, 6, 7, 8, 9, 10, 11},       // 8
    {0, 1, 4, 5, 6, 7, -1, 9, 10, 11},      // 9
};

void select_leds(int a)
{
    for (int i = 0; i < 10; i++)
    {
        int x = led_sets[a][i];

        if (x != -1)
            printf("LED no. %d ON\n", x);
    }
}
int main()
{
    while (1)
    {
        int x;

        scanf("%d", &x);

        select_leds(x);
    }

    return 0;
}