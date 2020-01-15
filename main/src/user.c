#include <yummyArmOS.h>

extern _task_manager taskmng;

void user_task_1() {
	int a = 1, b = 2, c = a + b;

	printf("TASK1 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
}

void user_task_2() {
	int a = 1, b = 2, c = a + b;

	printf("TASK2 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
}

void user_task_3() {
	int a = 1, b = 2, c = a + b;

	printf("TASK3 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
}

void user() {
	taskmng.create(user_task_1);
	taskmng.create(user_task_2);
	taskmng.create(user_task_3);
}
