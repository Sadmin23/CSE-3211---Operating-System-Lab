#include "seven_segment.h"
#include <stm32_peps.h>

void led_on(uint8_t a)
{
    GPIOA->BSRR |= (1U << a);
}

void led_off()
{
    for (uint8_t i = 0; i < 9; i++)
    {
        if (i != 2 && i != 3)
            GPIOA->BSRR |= (1U << (i + 16));
    }
}

uint8_t led_sets[10][7] = {
    {-1, 1, 4, 5, 6, 7, 8},     // 0
    {-1, -1, -1, 5, -1, -1, 8}, // 1
    {0, -1, 4, 5, 6, 7, -1},    // 2
    {0, -1, 4, 5, -1, 7, 8},    // 3
    {0, 1, -1, 5, -1, -1, 8},   // 4
    {0, 1, 4, -1, -1, 7, 8},    // 5
    {0, 1, 4, -1, 6, 7, 8},     // 6
    {-1, -1, 4, 5, -1, -1, 8},  // 7
    {0, 1, 4, 5, 6, 7, 8},      // 8
    {0, 1, 4, 5, -1, 7, 8},     // 9
};

void select_leds(uint8_t a)
{
    led_off();

    for (uint8_t i = 0; i < 7; i++)
    {
        uint8_t x = led_sets[a][i];

        if (x != -1)
            led_on(x);
    }
}
void led_init()
{
    RCC->AHB1ENR |= (1 << 0);

    for (uint8_t i = 0; i <= 9; i++)
    {
        if (i != 2 && i != 3)
            GPIOA->MODER |= (1U << 2 * i);
    }
}