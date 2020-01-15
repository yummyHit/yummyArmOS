#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <yummyArmOS.h>

#define MAX_MEM_BLOCK 40

typedef struct {
	unsigned int block_start_addr, block_end_addr;
	int is_used;
} _free_mem;

typedef struct {
	_free_mem free_mem_pool[MAX_MEM_BLOCK];
	void (*init)(void);
	unsigned int (*alloc)(void);
} _mem_manager;

void mem_init(void);
unsigned int mem_alloc(void);

#endif /* __MEMORY_H__ */
