#include <kmain.h>
#include <schedule.h>
#include <sem.h>

#define STOP 10000000

semaphore = 0;

TCB_TypeDef task[22], _sleep;
int count = 0;
int x = 10;

finished[task_count] = {0};
int spaces[6] = {10, 13, 15, 14, 15, 12};

void print_number(int index, int number)
{
    int space = spaces[index] - 5;

    if (number < 10)
        space += 3;
    else if (number < 100)
        space += 2;
    else if (number < 1000)
        space += 1;

    for (int i = 0; i < space; i++)
        kprintf(" ");

    kprintf("%dms |", number);
}

void print_report()
{
    uint16_t total_starting_time = 0;
    uint16_t total_response_time = 0;
    uint16_t total_completion_time = 0;
    uint16_t total_execution_time = 0;
    uint16_t total_waiting_time = 0;
    uint16_t total_turnaround_time = 0;

    kprintf("\n| Process Id | Start time | Response time | Completion time | Execution Time | Turnaround time | Waiting time |\n");

    for (int i = 0; i < task_count; i++)
    {
        uint16_t turnaround_time, waiting_time, completion_time, execution_time, starting_time, response_time;

        starting_time = task[i].starting_time;
        response_time = task[i].response_time;
        completion_time = task[i].completion_time;
        execution_time = task[i].execution_time;
        turnaround_time = completion_time - starting_time;
        waiting_time = turnaround_time - execution_time;

        kprintf("|       %d |", task[i].task_id);
        print_number(0, starting_time);
        print_number(1, response_time);
        print_number(2, completion_time);
        print_number(3, execution_time);
        print_number(4, turnaround_time);
        print_number(5, waiting_time);
        kprintf("\n");

        total_response_time += response_time;
        total_starting_time += starting_time;
        total_completion_time += completion_time;
        total_execution_time += execution_time;
        total_waiting_time += waiting_time;
        total_turnaround_time += turnaround_time;
    }

    total_response_time /= task_count;
    total_starting_time /= task_count;
    total_completion_time /= task_count;
    total_execution_time /= task_count;
    total_waiting_time /= task_count;
    total_turnaround_time /= task_count;

    kprintf("\n%dms %dms %dms %dms %dms %dms\n",
            total_starting_time,
            total_response_time,
            total_completion_time,
            total_execution_time,
            total_waiting_time,
            total_turnaround_time);
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
        // sem_inc(&semaphore);

        value = count;
        value++;

        if (value != count + 1)
        {
            printf("Task %d ", task.task_id);
            printf("Error %d != %d\n\r", value, count + 1); /* It is an SVC call*/
        }
        else
        {
            count = value;
            inc_count++;
        }

        // sem_dec(&semaphore);

        if (count >= STOP)
        {
            printf("Total increment done by task %d is: %d\n\r", task.task_id, inc_count);
            break;
        }
    }

    int index = task.task_id - 1000;
    finished[index] = 1;

    exit();
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
