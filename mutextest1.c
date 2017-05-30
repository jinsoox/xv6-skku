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

struct mutex_t m;
struct cond_t notfull, notempty;
int buffer[N];
int in=0, out=0, count=0;

void produce(int data){
	mutex_lock(&m);
	while(count == N)
		cond_wait(&notfull, &m);

	buffer[in] = data;
	in = (in+1)%N;
	count++;
	printf(1, "produce %d\n", data);

	cond_signal(&notempty);
	mutex_unlock(&m);
}

int consume(){
	int data;

	mutex_lock(&m);
	while(count == 0)
		cond_wait(&notempty, &m);

	data = buffer[out];
	out = (out+1)%N;
	count--;
	printf(1, "consume %d\n", data);

	cond_signal(&notfull);
	mutex_unlock(&m);

	return data;
}

void *producer(void *arg){
	int i;

	for(i=0;i<100;i++)
		produce(i);

	thread_exit(0);
}

void *consumer(void *arg){
	int i;

	for(i=0;i<100;i++)
		consume();

	thread_exit(0);
}

int
main(int argc, char **argv)
{
	int i;

	for(i=0;i<NTHREAD;i++)
		stack[i] = malloc(4096);

	mutex_init(&m);
	cond_init(&notfull);
	cond_init(&notempty);

	tid[0] = thread_create(producer, 20, 0, stack[0]);
	if(tid[0] == -1){
		printf(1, "thread creation failed\n");
		exit();
	}

	tid[1] = thread_create(consumer, 20, 0, stack[1]);
	if(tid[1] == -1){
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
