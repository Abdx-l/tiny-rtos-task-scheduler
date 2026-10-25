#pragma once
#include <stdint.h>
#define TASKS 5
typedef enum{UNUSED,READY,SLEEPING}State;
typedef struct{uint32_t*sp;State state;uint32_t delay;uint32_t stack[64];}TCB;
typedef struct{volatile unsigned available;}Semaphore;
extern TCB tasks[TASKS];extern unsigned current;
int task_create(void(*entry)(void));uint32_t*rtos_switch(uint32_t*sp);void SysTick_Handler(void);void task_delay(uint32_t ticks);int semaphore_try_take(Semaphore*s);void semaphore_give(Semaphore*s);void rtos_start(void);
