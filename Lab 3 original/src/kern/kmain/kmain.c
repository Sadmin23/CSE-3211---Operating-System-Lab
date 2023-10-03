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

#include <kmain.h>
#include <schedule.h>

#define STOP 1000000

TCB_TypeDef task[22], _sleep;
int global_count = 0;

void task_sleep(void)
{
    int reboot_ = 0;
    uprintf("\n\r\n\rAll tasks has been completed.\n\r\n\r\tEnter 1 to reboot the system. \n\r\tEnter any other number to enter sleeping mode.\n\r");
    uscanf("%d", &reboot_);
    if (reboot_ == 1)
        reboot();

    uprintf("Entering sleep mode...\n\r");
    while (1)
        ;
}

void Task(void)
{
    uint32_t value;
    uint32_t inc_count = 0;
    while (1)
    {
        uint16_t task_id = getpid() - 1000; /* It is an SVC call*/

        // critical region
        value = global_count;
        value++;

        // we check if some other tasks increase the count
        if (value != global_count + 1)
        {
            uprintf("Task %d running", task_id);
            uprintf("Error %d != %d\n\r", value, global_count + 1); /* It is an SVC call*/
        }
        else
        {
            // critical region
            // uprintf("Task %d running No Error %d == %d\n\r", task_id, value, global_count + 1); /* It is an SVC call*/
            global_count = value;
            inc_count++;
        }
        if (global_count >= STOP)
        {
            /* display how many increments it has successfully done!! */
            uprintf("Total increment done by task %d is: %d\n\r", task_id, inc_count);
            uprintf("Total increment done by task is: %d\n\r", inc_count);
            /* above is an SVC call */
            int fd = fopen("S_DISPLAY", 2);

            int x = 10;

            fprintf(fd, "%d\n", x);

            break;
        }
    }
    exit();
}

void unprivileged_mode(void)
{
    // read operation from special register CPSR and SPSR
    __asm volatile("MRS R0, CONTROL");
    __asm volatile("ORRS R0, R0, #1");
    // write operation to special register CPSR and SPSR
    __asm volatile("MSR CONTROL, R0");
}

void __set_interrupt_priorities(void)
{
    __NVIC_SetPriority(SVCall_IRQn, 1);
    __NVIC_SetPriority(SysTick_IRQn, 0x2);
    // lowest priority given to PendSV
    __NVIC_SetPriority(PendSV_IRQn, 0xFF);
}

void kmain(void)
{
    __sys_init();
    __set_interrupt_priorities();

    int task_count = 20;

    for (int i = 0; i < task_count; i++)
    {
        task_create(task + i, Task, (uint32_t *)(TASK_STACK_START - (i * TASK_STACK_SIZE)));
    }

    task_create(&_sleep, task_sleep, (uint32_t *)(TASK_STACK_START - (task_count * TASK_STACK_SIZE)));

    initialize_queue();

    for (int i = 0; i < task_count; i++)
        add_to_ready_queue(task + i);
    set_sleeping_task(&_sleep);

    // going to user mode
    unprivileged_mode();
    // set pendsv before starting task
    set_task_pending(1);

    // int lol = fopen("S_DISPLAY", 7);

    // int x = 10;

    // fprintf(5, "%d\n", x);

    // getfile();

    task_start();
    uprintf("\n\r\tAll Tasks Done!!!\n\r");

    getfile();

    while (1)
        ;
}
