#ifndef __SEM_H
#define __SEM_H
#include <types.h>

#define SIGNAL_UPDATE
#define WAIT_FOR_UPDATE WFI;
//     Indicate opportunity to enter low - power state

extern void sem_dec(const void *semaphore);
extern void sem_inc(const void *semaphore);

void atomic_increment(uint32_t *value);

void add_task(TCB_TypeDef task);
TCB_TypeDef get_task(void);

#endif