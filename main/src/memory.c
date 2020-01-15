#include <yummyArmOS.h>

_mem_manager memmng;

#define START_USER_STACK_ADDR	0xa0400000
#define USER_STACK_SIZE			0x00100000

unsigned int mem_alloc() {
	int i = 0;
	for(i = 0; i < MAX_MEM_BLOCK; i++) {
		if(memmng.free_mem_pool[i].is_used == 0) {
			memmng.free_mem_pool[i].is_used = 1;
			return memmng.free_mem_pool[i].block_end_addr;
		}
	}

	return 0;
}

void mem_init() {
	unsigned int pt = START_USER_STACK_ADDR;
	int i = 0;

	for(i = 0; i < MAX_MEM_BLOCK; i++) {
		memmng.free_mem_pool[i].block_start_addr = pt;
		memmng.free_mem_pool[i].block_end_addr = pt + USER_STACK_SIZE - 4;
		memmng.free_mem_pool[i].is_used = 0;
		pt += USER_STACK_SIZE;
	}

	memmng.init = mem_init;
	memmng.alloc = mem_alloc;
}
