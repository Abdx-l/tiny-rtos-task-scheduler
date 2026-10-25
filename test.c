#include "rtos.h"
#include <assert.h>
static void task(void){}
int main(void){assert(task_create(task)==0);assert(task_create(task)==1);current=0;task_delay(2);assert(tasks[0].state==SLEEPING);rtos_switch(tasks[0].sp);assert(current==1);SysTick_Handler();assert(tasks[0].state==SLEEPING);SysTick_Handler();assert(tasks[0].state==READY);Semaphore s={1};assert(semaphore_try_take(&s));assert(!semaphore_try_take(&s));semaphore_give(&s);assert(semaphore_try_take(&s));}
