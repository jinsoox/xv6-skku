#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "spinlock.h"
#include "proc.h"
#include "synch.h"

int mutex_init(struct mutex_t *mutex){
	return -1;
}

int mutex_lock(struct mutex_t *mutex){
	return -1;
}

int mutex_unlock(struct mutex_t *mutex){
	return -1;
}

int cond_init(struct cond_t *cond){
	return -1;
}

int cond_wait(struct cond_t *cond, struct mutex_t *mutex){
	return -1;
}

int cond_signal(struct cond_t *cond){
	return -1;
}
