.syntax unified
.cpu cortex-m3
.thumb
.global PendSV_Handler
.global port_start
.type PendSV_Handler,%function
PendSV_Handler:
 mrs r0,psp
 stmdb r0!,{r4-r11}
 push {r3,lr}
 bl rtos_switch
 pop {r3,lr}
 ldmia r0!,{r4-r11}
 msr psp,r0
 bx lr
.type port_start,%function
port_start:
 ldmia r0!,{r4-r11}
 msr psp,r0
 movs r0,#2
 msr control,r0
 isb
 pop {r0-r3,r12,lr}
 pop {r4}
 add sp,sp,#4
 bx r4
