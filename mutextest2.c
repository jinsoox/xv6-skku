#include "types.h"
#include "stat.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "spinlock.h"
#include "proc.h"
#include "synch.h"
#include "user.h"
#define N 8

void *stack[NTHREAD];
int tid[NTHREAD];
void *retval[NTHREAD];

struct mutex_t mutex;
struct cond_t cond;

struct com_data{
	int a;
	int b;
};

struct com_data mydata;

void *do_write(void *data){
	int a=1;
	int b=1;
	int t;
	
	while(1){
		mutex_lock(&mutex);
		mydata.a = a;
		mydata.b = b;
		t = a+b;
		a = b;
		b = t;
		cond_signal(&cond);
		mutex_unlock(&mutex);
		sleep(100);
	}
}

void *do_read(void *data){
	while(1){
		mutex_lock(&mutex);
		cond_wait(&cond, &mutex);
		printf(1, "a: %d, b:%d\n", mydata.a, mydata.b);
		mutex_unlock(&mutex);
	}
}

int
main(int argc, char **argv)
{
	int i;

	mutex_init(&mutex);
	cond_init(&cond);

	for(i=0;i<NTHREAD;i++)
		stack[i] = malloc(4096);

	if((tid[0] = thread_create(do_write, 30, 0, stack[0])) == -1){
		printf(1, "thread creation failed\n");
		exit();
	}

	if((tid[1] = thread_create(do_read, 10, 0, stack[1])) == -1){
		printf(1, "thread creation failed\n");
		exit();
	}

	if(thread_join(tid[0], &retval[0]) == -1){
		printf(1, "thread join failed\n");
		exit();
	}

	if(thread_join(tid[1], &retval[1]) == -1){
		printf(1, "thread join failed\n");
		exit();
	}

	for(i=0;i<NTHREAD;i++)
		free(stack[i]);

	exit();
}
