/*
 * Copyright (c) 2022
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys_init.h>
#include <cm4.h>
#include <kmain.h>
#include <kstdio.h>
#include <kstring.h>
#include <stdint.h>
#include <usart.h>
// #include "../include/float.h"

void led_on(int a)
{
	GPIOA->ODR |= (1U << a);
}

void led_off()
{
	int i;

	for (i = 0; i < 12; i++)
	{
		if (i != 2 && i != 3)
			GPIOA->ODR &= (1U << i);
	}
}

int led_sets[10][10] = {
	{0, 1, 4, 5, 6, -1, 8, 9, 10, 11},		// 0
	{-1, -1, -1, 5, -1, -1, -1, 9, -1, -1}, // 1
	{0, 1, -1, 5, 6, 7, 8, -1, 10, 11},		// 2
	{0, 1, -1, 5, 6, 7, -1, 9, 10, 11},		// 3
	{0, -1, 4, 5, 6, 7, -1, 9, -1, -1},		// 4
	{0, 1, 4, -1, 6, 7, -1, 9, 10, 11},		// 5
	{0, 1, 4, -1, 6, 7, 8, 9, 10, 11},		// 6
	{0, 1, -1, 5, -1, -1, -1, 9, -1, -1},	// 7
	{0, 1, 4, 5, 6, 7, 8, 9, 10, 11},		// 8
	{0, 1, 4, 5, 6, 7, -1, 9, 10, 11},		// 9
};

void select_leds(int a)
{
	led_off();

	for (int i = 0; i < 10; i++)
	{
		int x = led_sets[a][i];

		if (x != -1)
			led_on(x);
	}
}

void kmain(void)
{
	__sys_init();

	RCC->AHB1ENR |= (1 << 0);

	int i;

	for (i = 0; i < 12; i++)
	{
		if (i != 2 && i != 3)
			GPIOA->MODER |= (1U << 2 * i);
	}

	kprintf("After Init\n");

	while (1)
	{
		int b;
		kscanf("%d", &b);
		kprintf("%d", b);

		select_leds(b);
	}
}
