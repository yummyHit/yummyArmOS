#include <yummyArmOS.h>

extern _mem_manager memmng;
_task_manager taskmng;

#define START_USER_CPSR		0x68000050

int task_create(void(*start_func)(void)) {
	int task_index = 0;
	unsigned int stack_top = 0;

	taskmng.max_task_id++;
	task_index = taskmng.max_task_id;

	if(task_index >= MAX_TASK_NUM)
		return -1;

	stack_top = memmng.alloc();

	if(stack_top == 0)
		return -2;

	taskmng.free_task_pool[task_index].context_spsr = START_USER_CPSR;
	taskmng.free_task_pool[task_index].context_sp = stack_top;
	taskmng.free_task_pool[task_index].context_pc = (unsigned int)start_func;

	return task_index;
}

void task_init() {
	int i = 0;
	for(i = 0; i < MAX_TASK_NUM; i++) {
		taskmng.free_task_pool[i].context_spsr = 0x00;
		memset(taskmng.free_task_pool[i].context, 0, sizeof(unsigned int) * CONTEXT_NUM);
		taskmng.free_task_pool[i].context_sp = 0x00;
		taskmng.free_task_pool[i].context_lr = 0x00;
		taskmng.free_task_pool[i].context_pc = 0x00;
	}

	taskmng.max_task_id = -1;
	taskmng.init = task_init;
	taskmng.create = task_create;
}
