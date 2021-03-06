/*========================================================================================================================

  화일명 : entry.S
  설  명 : 램 상에 존재하는 ezBoot의 메인 C 함수의 main을 호출하기 위한 루틴이다.
  
  작성자 : (주)제이닷디앤티  유영창
  작성일 : 2001년 10월 8일
  
  비  고 :  
==========================================================================================================================*/
.globl _ram_entry
_ram_entry:
	b		kernel_init
	b		_ram_entry // main 함수의 호출이 끝났다면 재 호출한다. 
	b		asm_swi_handler
	b		asm_irq_handler

#define svc_stack	0xa0300000
#define irq_stack	0xa0380000
#define sys_stack	0xa0400000

.global kernel_init
kernel_init:
	msr		cpsr_c,#0xc0|0x13
	ldr		r0,=svc_stack
	sub		sp,r0,#4

	msr		cpsr_c,#0xc0|0x12
	ldr		r0,=irq_stack
	sub		sp,r0,#4

	msr		cpsr_c,#0xc0|0x1f
	ldr		r0,=sys_stack
	sub		sp,r0,#4

	msr		cpsr_c,#0xc0|0x13

	bl		main	   // C 함수의 main함수를 호출한다. 	
	b		_ram_entry

.global asm_swi_handler
asm_swi_handler:
	stmfd	sp!,{r0-r12,r14}
	mrs		r1,spsr
	stmfd	sp!,{r1}
	ldr		r10,[lr,#-4]
	bic		r10,r10,#0xff000000
	mov		r0,r10
	bl		swi_handler
	ldmfd	sp!,{r1}
	msr		spsr_cxsf,r1
	ldmfd	sp!,{r0-r12,pc}^

.global asm_irq_handler
asm_irq_handler:
	sub		lr,lr,#4
	stmfd	sp!,{lr}
	stmfd	sp!,{r0-r14}^
	mrs		r1,spsr
	stmfd	sp!,{r1}
	bl		irq_handler
	ldmfd	sp!,{r1}
	msr		spsr_cxsf,r1
	ldmfd	sp!,{r0-r14}^
	ldmfd	sp!,{pc}^
