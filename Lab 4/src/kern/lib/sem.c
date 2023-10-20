#include <sem.h>
#include <schedule.h>

// sem_dec
// Declare for use from C as extern void sem_dec(void * semaphore);
void sem_dec(const void *semaphore)
{
    asm volatile("ldrex r1, [ %0 ] \n"     // s'= MEM[ &s ]
                 "cmp    r1, #0 \n"        // s'?= 0
                 "beq    sem_dec \n"      // if s'== 0, retry
                 "sub    r1, r1, #1 \n"    // s'= s'- 1
                "strex r2, r1, [ %0 ] \n" // r <= MEM[ &s ] = s'
                //  "cmp    r2, #0 \n"        // r  ?= 0
                //  "bne    sem_dec \n"      // if r  != 0, retry
                //  "dmb \n"                  // memory barrier
                //  "bx     lr \n"            // return
                 :
                 : "r"(semaphore)
                 : "r0", "r1", "r2");

    return;
}

// sem_inc
// Declare for use from C as extern void sem_inc(void * semaphore);
void sem_inc(const void *semaphore)
{
    asm volatile("ldrex r1, [ %0 ] \n"     // s'= MEM[ &s ]
                 "add    r1, r1, #1 \n"    // s'= s'+ 1
                 "strex r2, r1, [ %0 ] \n" // r <= MEM[ &s ] = s'
                //  "cmp    r2, #0 \n"        // r  ?= 0
                //  "bne    sem_inc \n"      // if r  != 0, retry
                //  "dmb \n"                  // memory barrier
                //  "bx     lr \n"            // return
                 :
                 : "r"(semaphore)
                 : "r0", "r1", "r2");

    return;
}

unsigned int task_semaphore = 0;

void add_task(TCB_TypeDef task)
{
    add_to_blocked_queue(task);

    sem_inc(&task_semaphore);

    return;
}
TCB_TypeDef get_task(void)
{
    sem_dec(&task_semaphore);

    TCB_TypeDef tmptask = blocked_queue_front_();

    return tmptask;
}