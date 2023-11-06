#include <kmain.h>
#include <schedule.h>
#include <sem.h>

#define STOP 1000000

int semaphore = 0;

TCB_TypeDef task[22], _sleep;
int count = 0;
int flag = 0;
int task_count = 10;
int x = 10;

void print_report()
{
    for (int i = 0; i < task_count; i++)
    {
        printf("%dms %dms %dms %dms\n", task[i].starting_time, task[i].response_time, task[i].completion_time, task[i].execution_time);
    }
}

void task_sleep(void)
{
    print_report();
    int reboot_ = 0;
    printf("\nAll tasks has been completed.\nENTER 1 TO REBOOT DUOS. \nENTER 0 TO SHUTDOWN\n\r");
    uscanf("%d", &reboot_);
    if (reboot_ == 1)
        reboot();

    printf("SHUTTING DOWN DUOS...\n\r");
    while (1)
        ;
}

void Task(void)
{
    TCB_TypeDef task = getpid(); /* It is an SVC call*/

    uint32_t value;
    uint32_t inc_count = 0;
    while (1)
    {

        // while (flag != 0)
        //     ;

        // flag = 1;

        // sem_inc(&semaphore);

        // add_task(task);

        value = count;
        value++;

        if (value != count + 1)
        {
            // printf("Task %d ", task.task_id);
            // printf("Error %d != %d\n\r", value, count + 1); /* It is an SVC call*/
        }
        else
        {
            count = value;
            inc_count++;
        }

        // flag = 0;

        // sem_dec(&semaphore);

        if (count >= STOP)
        {
            // printf("Total increment done by task %d is: %d\n\r", task.task_id, inc_count);
            // printf("Total increment done by task is: %d\n\r", inc_count);

            break;
        }

        // task = get_task();
    }

    exit();
}

void unprivileged_mode(void)
{
    __asm volatile("MRS R0, CONTROL");
    __asm volatile("ORRS R0, R0, #1");
    __asm volatile("MSR CONTROL, R0");
}

void kmain(void)
{
    __sys_init();

    __NVIC_SetPriority(SVCall_IRQn, 1);
    __NVIC_SetPriority(SysTick_IRQn, 0x2);
    __NVIC_SetPriority(PendSV_IRQn, 0xFF);

    for (int i = 0; i < task_count; i++)
    {
        task_create(task + i, Task, (uint32_t *)(TASK_STACK_START - (i * TASK_STACK_SIZE)));
    }

    task_create(&_sleep, task_sleep, (uint32_t *)(TASK_STACK_START - (task_count * TASK_STACK_SIZE)));

    initialize_queue();

    for (int i = 0; i < task_count; i++)
        add_to_ready_queue(task + i);
    set_sleeping_task(&_sleep);

    set_task_pending(1);

    task_start();

    while (1)
        ;
}
