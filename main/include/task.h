#ifndef __TASK_H__
#define __TASK_H__

#define MAX_TASK_NUM	40
#define CONTEXT_NUM		13

typedef struct {
	unsigned int context_spsr;
	unsigned int context[CONTEXT_NUM];
	unsigned int context_sp;
	unsigned int context_lr;
	unsigned int context_pc;
} _free_task;

typedef struct {
	_free_task free_task_pool[MAX_TASK_NUM];
	int max_task_id;

	void (*init)(void);
	int (*create)(void(*start_func)(void));
} _task_manager;

void task_init(void);
int task_create(void(*start_func)(void));

#endif /* __TASK_H__ */
