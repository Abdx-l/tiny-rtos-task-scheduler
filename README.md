# Tiny RTOS Task Scheduler

Static ARM Cortex-M3 task scheduler with per-task stacks, SysTick wakeups, PendSV r4-r11 context switching, an idle task, delay calls and binary semaphore primitives. Host tests exercise state scheduling; ARM assembly is cross-compiled in CI.

## Run

Host tests: `cc -DHOST_TEST -std=c11 test.c scheduler.c -o scheduler_test && ./scheduler_test`. ARM: `arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -c scheduler.c port.S`. Board startup must set VTOR, configure SysTick, set PendSV to lowest priority, call rtos_start with interrupts enabled, and supply UART/GPIO routines.
