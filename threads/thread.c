#include <assert.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"
#include "interrupt.h"

#define EMPTY_ID -1


/* This is the wait queue structure */
struct wait_queue {
	/* ... Fill this in Lab 3 ... */
};

/* Store the states of a thread */
enum THREAD_STATE {
    EMPTY,
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
} thread;

thread thread_queue[THREAD_MAX_THREADS];

/* thread starts by calling thread_stub. The arguments to thread_stub are the
 * thread_main() function, and one argument to the thread_main() function. 
 * NOTE: This still needs to be changed. */
void
thread_stub(void (*thread_main)(void *), void *arg)
{
	Tid ret;

	thread_main(arg); // call thread_main() function with arg
	thread_exit();
}
	
void
thread_init(void)
{
    /* Initialize thread queue element to default values */
    for (int i=0; i<THREAD_MAX_THREADS; i++) {
        thread_queue[i].id = EMPTY_ID;
        thread_queue[i].state = EMPTY;
        thread_queue[i].context = NULL;
    }

    /* Define a ucontext_t structure to represent the currently running context */
    struct ucontext_t *cur = (struct ucontext_t*)malloc(sizeof(struct ucontext_t));
    int err = getcontext(cur);
    if (err < 0) {
        printf("getcontext() failed in thread_init\n");
        exit(EXIT_FAILURE);
    }

    /* Define thread Tid = 0 in the queue to the current context */
    thread_queue[0].id = (Tid)0;
    thread_queue[0].state = RUNNING;
    thread_queue[0].context = cur;

    return;
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
    int ret;

    if (want_tid == THREAD_SELF) { // Continue the execution of the caller (thread in the current context) & return the Tid of the current thread
        return thread_id();

    } else if (want_tid == THREAD_ANY) { // Execute the next available thread in the Ready queue

        int next_id = EMPTY_ID;
        int curr_id = (int)thread_id(); // Placeholder to store the current thread's ID

        // Determine index/threadID of next available READY state thread
        for (int i; i<THREAD_MAX_THREADS; i++) {
            if (thread_queue[i].state == READY) {
                // Validate that index and ID match
                if (thread_queue[i].id != (Tid)i) {
                    printf("Thread ID and index failure in thread_yield\n");
                    exit(EXIT_FAILURE);
                }
                next_id = i;
            }
        }

        // Check status of READY thread
        if (next_id == EMPTY_ID) {
            return THREAD_NONE;
        } else {

            // Set current thread into Ready state
            getcontext(thread_queue[curr_id].context); // Update context information for this thread
            thread_queue[curr_id].state = READY;

            // Set the next thread into Run state
            thread_queue[next_id].state = RUNNING;
            ret = setcontext(thread_queue[curr_id].context);
            if (ret < 0) {
                return THREAD_INVALID;
            }

            return (Tid)next_id;
        }

    } else { // Execute the thread specified by want_tid w/ error checking
        TBD();
    }

	return THREAD_FAILED;
}

/* // DEBUG REMOVE - Used for testing the code base during development.
int main() {
    // Initialized thread
    thread_init();
    
    Tid ret;
    
    // Test that return is THREAD_NONE
    ret = thread_yield(THREAD_ANY);
    assert(ret == THREAD_NONE);
    printf("Test 1: return THREAD_NONE passed!");

    // Test return is 0
    ret = thread_yield(THREAD_SELF);
    assert(ret == 0);
    printf("Test 2: return THREAD_SELF passed!");

    return 0;
}
 */
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
