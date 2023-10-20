#include <sem.h>
#include <schedule.h>

// sem_dec
// Declare for use from C as extern void sem_dec(void * semaphore);
extern void sem_dec(const void *semaphore)
{
    __asm volatile("1: LDREX R1, [%0]"
                   :
                   : "r"(semaphore));
    __asm volatile("CMP R1, #0");
    __asm volatile("BEQ 1b");
    __asm volatile("SUB R1, #1");
    __asm volatile("STREX R2,R1,[%0]"
                   : "=r"(semaphore));
    __asm volatile("CMP R2, #0");
    __asm volatile("BNE 1b");
    __asm volatile("DMB");
    __asm volatile("BX LR");
    __asm volatile("BX LR");
    __asm volatile("BX LR");

    return;
}
// sem_inc
// Declare for use from C as extern void sem_inc(void * semaphore);
extern void sem_inc(const void *semaphore)
{
    __asm volatile("1: LDREX R1, [%0]"
                   :
                   : "r"(semaphore));
    __asm volatile("ADD R1, #1");
    __asm volatile("STREX R2,R1,[%0]"
                   : "=r"(semaphore));
    __asm volatile("CMP R2, #0");
    __asm volatile("BNE 1b");
    __asm volatile("DMB");
    __asm volatile("BX LR");

    return;
}

unsigned int task_semaphore = 0;

void add_task(TCB_TypeDef task)
{
    add_to_blocked_queue(task);

    // sem_inc(&task_semaphore);

    return;
}
TCB_TypeDef get_task(void)
{
    // sem_dec(&task_semaphore);

    TCB_TypeDef tmptask = blocked_queue_front_();

    return tmptask;
}