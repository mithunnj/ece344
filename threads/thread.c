#include <assert.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"
#include "interrupt.h"


/* This is the wait queue structure */
struct wait_queue {
	/* ... Fill this in Lab 3 ... */
};

/* Store the states of a thread */
enum THREAD_STATE {
    READY,
    RUNNING,
    BLOCKED,
    EXITED
};

/* This is the thread control block */
typedef struct thread {
    Tid id;
    int state;
    struct ucontext_t *context;
}thread;

thread thread_queue[THREAD_MAX_THREADS];
	
void
thread_init(void)
{
    /* Define a ucontext_t structure to represent the currently running context */
    struct ucontext_t *cur = (struct ucontext_t*)malloc(sizeof(struct ucontext_t));
    int err = getcontext(cur);
    if (err < 0) {
        printf("getcontext() failed in thread_init\n");
        exit(EXIT_FAILURE);
    }

    /* Define thread Tid = 0 in the queue to the current context */
    Tid main_id = 0;
    for (int i=0; i<THREAD_MAX_THREADS; i++) {
        if (thread_queue[i].context == NULL) { // If we have an empty slot in the thread queue
            thread_queue[i].id = main_id;
            thread_queue[i].state = RUNNING;
            thread_queue[i].context = cur;
        }
    }

    return;

}

int main() {
    thread_init();

    for (int i=0; i<THREAD_MAX_THREADS; i++) {
        if (thread_queue[i].context != NULL) {
            printf("\nIndex: %d\n", i);
            printf("id: %d\n", thread_queue[i].id);
            printf("state: %d\n", thread_queue[i].state);
            printf("context ptr: %p\n", thread_queue[i].context);
            printf("end test for ind: %d\n", i);
        }
    
    }

    return 0;
}

Tid
thread_id()
{
    /* Loop through threads in queue, and return the index of thread in the Running state. */
    for (int i=0; i<THREAD_MAX_THREADS; i++) {
        if (thread_queue[i].state == RUNNING) {
            return (Tid)i;
        }
    }

	return THREAD_INVALID;
}

Tid
thread_create(void (*fn) (void *), void *parg)
{
	TBD();
	return THREAD_FAILED;
}

Tid
thread_yield(Tid want_tid)
{
    if (want_tid == THREAD_SELF) { // Continue the execution of the caller (thread in the current context) & return the Tid of the current thread
        return thread_id();
    } else if (want_tid == THREAD_ANY) { // Execute the next available thread in the Ready queue
        TBD();
    } else { // Execute the thread specified by want_tid w/ error checking
        TBD();
    }

	return THREAD_FAILED;
}

void
thread_exit()
{
	TBD();
}

Tid
thread_kill(Tid tid)
{
	TBD();
	return THREAD_FAILED;
}

/*******************************************************************
 * Important: The rest of the code should be implemented in Lab 3. *
 *******************************************************************/

/* make sure to fill the wait_queue structure defined above */
struct wait_queue *
wait_queue_create()
{
	struct wait_queue *wq;

	wq = malloc(sizeof(struct wait_queue));
	assert(wq);

	TBD();

	return wq;
}

void
wait_queue_destroy(struct wait_queue *wq)
{
	TBD();
	free(wq);
}

Tid
thread_sleep(struct wait_queue *queue)
{
	TBD();
	return THREAD_FAILED;
}

/* when the 'all' parameter is 1, wakeup all threads waiting in the queue.
 * returns whether a thread was woken up on not. */
int
thread_wakeup(struct wait_queue *queue, int all)
{
	TBD();
	return 0;
}

/* suspend current thread until Thread tid exits */
Tid
thread_wait(Tid tid)
{
	TBD();
	return 0;
}

struct lock {
	/* ... Fill this in ... */
};

struct lock *
lock_create()
{
	struct lock *lock;

	lock = malloc(sizeof(struct lock));
	assert(lock);

	TBD();

	return lock;
}

void
lock_destroy(struct lock *lock)
{
	assert(lock != NULL);

	TBD();

	free(lock);
}

void
lock_acquire(struct lock *lock)
{
	assert(lock != NULL);

	TBD();
}

void
lock_release(struct lock *lock)
{
	assert(lock != NULL);

	TBD();
}

struct cv {
	/* ... Fill this in ... */
};

struct cv *
cv_create()
{
	struct cv *cv;

	cv = malloc(sizeof(struct cv));
	assert(cv);

	TBD();

	return cv;
}

void
cv_destroy(struct cv *cv)
{
	assert(cv != NULL);

	TBD();

	free(cv);
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
	assert(cv != NULL);
	assert(lock != NULL);

	TBD();
}

void
cv_signal(struct cv *cv, struct lock *lock)
{
	assert(cv != NULL);
	assert(lock != NULL);

	TBD();
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
	assert(cv != NULL);
	assert(lock != NULL);

	TBD();
}
