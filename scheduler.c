#include "rtos.h"
#include <stddef.h>
TCB tasks[TASKS];unsigned current;
#ifndef HOST_TEST
#define ICSR (*(volatile uint32_t*)0xe000ed04)
static uint32_t critical(void){uint32_t mask;__asm volatile("mrs %0,primask\ncpsid i":"=r"(mask)::"memory");return mask;}
static void restore(uint32_t mask){__asm volatile("msr primask,%0"::"r"(mask):"memory");}
#else
static uint32_t critical(void){return 0;}static void restore(uint32_t mask){(void)mask;}
#endif
static void request_switch(void){
#ifndef HOST_TEST
 ICSR=1u<<28;
#endif
}
static void idle(void){for(;;){
#ifndef HOST_TEST
 __asm volatile("wfi");
#endif
}}
int task_create(void(*entry)(void)){
 uint32_t key=critical();
 for(unsigned i=0;i<TASKS;i++)if(tasks[i].state==UNUSED){
  uint32_t*sp=&tasks[i].stack[64];
  *(--sp)=0x01000000;*(--sp)=(uint32_t)(uintptr_t)entry;*(--sp)=(uint32_t)(uintptr_t)idle;
  for(int j=0;j<13;j++)*(--sp)=0;
  tasks[i].sp=sp;tasks[i].delay=0;tasks[i].state=READY;restore(key);return (int)i;
 }restore(key);return -1;
}
uint32_t*rtos_switch(uint32_t*sp){
 if(sp)tasks[current].sp=sp;
 for(unsigned i=1;i<=TASKS;i++){unsigned next=(current+i)%TASKS;if(tasks[next].state==READY){current=next;return tasks[next].sp;}}
 return tasks[current].sp;
}
void SysTick_Handler(void){for(unsigned i=0;i<TASKS;i++)if(tasks[i].state==SLEEPING&&tasks[i].delay&&--tasks[i].delay==0)tasks[i].state=READY;request_switch();}
void task_delay(uint32_t ticks){uint32_t key=critical();if(ticks){tasks[current].delay=ticks;tasks[current].state=SLEEPING;}request_switch();restore(key);}
int semaphore_try_take(Semaphore*s){uint32_t key=critical();int got=s->available!=0;if(got)s->available=0;restore(key);return got;}
void semaphore_give(Semaphore*s){uint32_t key=critical();s->available=1;restore(key);}
void rtos_start(void){task_create(idle);
#ifndef HOST_TEST
 extern void port_start(uint32_t*);port_start(tasks[current].sp);
#endif
}
