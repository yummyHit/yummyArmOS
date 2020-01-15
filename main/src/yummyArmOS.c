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
#endif
