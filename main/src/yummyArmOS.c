//------------------------------------------------------------------------------
// 화일명 : main.c
// 설  명 : ezBoot의 실제적인 메인 루틴이다. 
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
//          오재경 (주)제이닷디엔티 freefrug@falinux.com -- 푸지
// 작성일 : 2001년 10월 08일
// 수정일 : 2003년 06월 07일 	ez_x5 용으로 수정되었다.
//          2003년 08월 01일 	eztiny 를 추가    			        -- 푸지
//	    2003년 08월 02일	nand.c 타이밍 수정, 연속된 bad-block 버그 수정  -- 푸지
//	    2004년 08월 14일	AM29LV400BT 플래시 추가  			-- 푸지	
// 저작권 : (주)제이닷디엔티 
// 주  의 : 
//------------------------------------------------------------------------------

#if CHAPTER2
#include <pxa255.h>
#include <time.h>
#include <gpio.h>
#include <stdio.h>
#include <string.h>

int main() {
	while(1) {
		printf("hello world\n");
		msleep(1000);
	}
}
#elif CHAPTER3
#include <yummyArmOS.h>

int main() {
	int i = 0;
	printf("hello world\n");
	msleep(1000);

	while(1) {
		GPIO_SetLED(i, LED_OFF);
		msleep(500);
		GPIO_SetLED(i, LED_ON);
		msleep(500);
		i++;
		if(i > 3) i = 0;
	}
}
#elif CHAPTER4
#include <yummyArmOS.h>

void swi_handler(unsigned int num) {
	printf("system call %d\n", num);
}

void irq_handler() {
}

int main() {
	__asm__("swi 77");
	return 0;
}
#elif CHAPTER5
#include <yummyArmOS.h>

void swi_handler(unsigned int num) {
	printf("system call %d\n", num);
}

void irq_handler() {
}

int main() {
	while(1) {
		__asm__("swi 77");
		msleep(1000);
	}

	return 0;
}
#elif CHAPTER6
#include <yummyArmOS.h>

void swi_handler(unsigned int num) {
	printf("system call %d\n", num);
}

void irq_handler() {
	if((ICIP & (1 << 27)) != 0) {
		OSSR = OSSR_M1;
		OSMR1 = OSCR + 3686400;
		printf("Timer Interrupt !!\n");
	}
}

void os_timer_init() {
	ICCR = 0x01;
	ICMR |= (1 << 27);
	ICLR &= ~(1 << 27);
	OSCR = 0;
	OSMR1 = OSCR + 3686400;
	OSSR = OSSR_M1;
}

void os_timer_start() {
	OIER |= (1 << 1);
	OSSR = OSSR_M1;
}

void irq_enable() {
	__asm__("msr    cpsr_c, #0x40|0x13");
}

void irq_disable() {
	__asm__("msr    cpsr_c, #0xc0|0x13");
}

int main() {
	os_timer_init();
	os_timer_start();

	irq_enable();

	while(1) {
		__asm__("swi 77");
		msleep(1000);
	}

	return 0;
}
#elif CHAPTER7
#include <yummyArmOS.h>

void swi_handler(unsigned int num) {
	printf("system call %d\n", num);
}

void irq_handler() {
	if((ICIP & (1 << 27)) != 0) {
		OSSR = OSSR_M1;
		OSMR1 = OSCR + 3686400;
		printf("Timer Interrupt !!\n");
	}
}

void os_timer_init() {
	ICCR = 0x01;
	ICMR |= (1 << 27);
	ICLR &= ~(1 << 27);
	OSCR = 0;
	OSMR1 = OSCR + 3686400;
	OSSR = OSSR_M1;
}

void os_timer_start() {
	OIER |= (1 << 1);
	OSSR = OSSR_M1;
}

void irq_enable() {
	__asm__("msr    cpsr_c, #0x40|0x13");
}

void irq_disable() {
	__asm__("msr    cpsr_c, #0xc0|0x13");
}

int main() {
	int a = 1, b = 2, c = a + b;
	os_timer_init();
	os_timer_start();

	irq_enable();

	while(1) {
		printf("kernel stack a(%p), b(%p), c(%p)\n", &a, &b, &c);
		__asm__("swi 77");
		msleep(1000);
	}

	return 0;
}
#elif CHAPTER9
#include <yummyArmOS.h>

extern _mem_manager memmng;
extern _task_manager taskmng;

void swi_handler(unsigned int num) {
	printf("system call %d\n", num);
}

void irq_handler() {
	if((ICIP & (1 << 27)) != 0) {
		OSSR = OSSR_M1;
		OSMR1 = OSCR + 3686400;
		printf("Timer Interrupt !!\n");
	}
}

void os_timer_init() {
	ICCR = 0x01;
	ICMR |= (1 << 27);
	ICLR &= ~(1 << 27);
	OSCR = 0;
	OSMR1 = OSCR + 3686400;
	OSSR = OSSR_M1;
}

void os_timer_start() {
	OIER |= (1 << 1);
	OSSR = OSSR_M1;
}

void irq_enable() {
	__asm__("msr    cpsr_c, #0x40|0x13");
}

void irq_disable() {
	__asm__("msr    cpsr_c, #0xc0|0x13");
}

void init() {
	mem_init();
	task_init();

	os_timer_init();
	os_timer_start();
}

int main() {
	init();
	user();

	irq_enable();

	int i = 0;
	for(i = 0; i <= taskmng.max_task_id; i++) {
		printf("TCB: TASK%d - init PC(%p)\t init SP(%p)\n", i+1, taskmng.free_task_pool[i].context_pc, taskmng.free_task_pool[i].context_sp);
	}

	printf("REAL function TASK1: %p\n", user_task_1);
	printf("REAL function TASK2: %p\n", user_task_2);
	printf("REAL function TASK3: %p\n", user_task_3);

	while(1) {
		msleep(1000);
	}

	return 0;
}
#endif
