#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <ucontext.h>
#include "interrupt.h"

#ifndef __x86_64__
#error "Do this project on a 64-bit x86-64 linux machine"
#endif /* __x86_64__ */

#if __WORDSIZE != 64
#error "word size should be 64 bits"
#endif

#define DEBUG 1

static void call_setcontext(ucontext_t * context);
static void show_interrupt(void);

/* zero out the context */

/* Userlevel context.  */
/*
  type = struct ucontext_t {
    unsigned long uc_flags;
    struct ucontext_t *uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    sigset_t uc_sigmask;
    struct _libc_fpstate __fpregs_mem;
    unsigned long long __ssp[4];
}

*/
ucontext_t mycontext = { 0 };

void
get_start_end(long *startp, long *endp)
{
    /* A file containing the currently mapped memory regions and
              their access permissions.  See mmap(2)
    More info: https://man7.org/linux/man-pages/man5/proc.5.html
    */
	char filename[128];
	char line[256];
	FILE *f;

	sprintf(filename, "/proc/%d/maps", getpid());
	f = fopen(filename, "r");
	assert(f);
	/* read first line */
	fgets(line, sizeof(line), f);
	/* read second line, which has the executable region. */
	fgets(line, sizeof(line), f);
	sscanf(line, "%lx-%lx ", startp, endp);
}

int
main(int argc, char **argv)
{
	long start, end;
	/* we declare this variable to be volatile so that the compiler will
	 * make sure to store it on the stack and not in a register. This is
	 * ESSENTIAL, or else the code in this function may run in an infinite
	 * loop (you can try this by removing the volatile keyword, and
	 * compiling this file with the "-O2" flag to the gcc compiler, by
	 * changing CFLAGS in the Makefile).
	 *
	 * QUESTION: why must setcontext_called be stored on the stack, and not
	 * in a register? You will need to look at the code below, and
	 * understand how getcontext and setcontext work to answer this
	 * question.
     *
     * ANSWER: As described here: https://www.geeksforgeeks.org/understanding-volatile-qualifier-in-c/, when certain
     * optimizations are turned on during the C compilation, some variables are stored in memory and do not behave as expected
     * in multi-threaded scenarios. So this tells the compiler to expect the variable to change its value in an unexpected way, like
     * in this case setcontext_called is defined as 0 in the main thread, and then the variable is defined as 1 for the mycontext
     * thread.
     * */
	volatile int setcontext_called = 0;
	int err;

	/*
	 * DO NOT CHANGE/ADD ANY CODE UNTIL BELOW TBD(). SEE BELOW.
	 */

	/* Get context: make sure to read the man page of getcontext in detail,
	 * or else you will not be able to answer the questions below. */
	err = getcontext(&mycontext);
	assert(!err);

	/* QUESTION: which of the fields of mycontext changed due to the call
	 * above? Hint: It will help to run the program using gdb and put a
	 * breakpoint at entry to main and before and after the calls to
	 * getcontext().
	 * - Use "info registers" to see the values of the registers.
	 * - Use "next"/"step" to advance to the next line of code.
	 * - Use "print mycontext" to see the values stored in mycontext.
	 *   Compare them with the output of "info registers".
	 * - Use "ptype mycontext" so see the type/fields of mycontext
     *
     *   ANSWER:
     *
     *   BEFORE:
     *
     *   {uc_flags = 0, uc_link = 0x0, uc_stack = {ss_sp = 0x0, ss_flags = 0,
    ss_size = 0}, uc_mcontext = {gregs = {0 <repeats 23 times>}, fpregs = 0x0,
    __reserved1 = {0, 0, 0, 0, 0, 0, 0, 0}}, uc_sigmask = {__val = {
      0 <repeats 16 times>}}, __fpregs_mem = {cwd = 0, swd = 0, ftw = 0,
    fop = 0, rip = 0, rdp = 0, mxcsr = 0, mxcr_mask = 0, _st = {{
        significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0,
          0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {
          0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0,
        __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0},
        exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0,
          0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0,
          0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {
        significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0,
          0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {
          0, 0, 0}}}, _xmm = {{element = {0, 0, 0, 0}} <repeats 16 times>},
    __glibc_reserved1 = {0 <repeats 24 times>}}, __ssp = {0, 0, 0, 0}}

    AFTER:

     {uc_flags = 0, uc_link = 0x0, uc_stack = {ss_sp = 0x0, ss_flags = 0,
    ss_size = 0}, uc_mcontext = {gregs = {140737353751936, 140737353751936, 0,
      0, 93824992235984, 140737488347792, 0, 0, 93824992264480,
      140737488347800, 140737488347568, 0, 140737488347816, 0,
      140737353746200, 140737488347520, 93824992236462, 0, 0, 0, 0, 0, 0},
    fpregs = 0x55555555c2c8 <mycontext+424>, __reserved1 = {0, 0, 0, 0, 0, 0,
      0, 0}}, uc_sigmask = {__val = {0 <repeats 16 times>}}, __fpregs_mem = {
    cwd = 895, swd = 65535, ftw = 0, fop = 65535, rip = 4294967295, rdp = 0,
    mxcsr = 8064, mxcr_mask = 0, _st = {{significand = {0, 0, 0, 0},
        exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0,
          0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0,
          0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {
        significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0,
          0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {
          0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0,
        __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0},
        exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0,
          0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}}, _xmm = {{
        element = {0, 0, 0, 0}} <repeats 16 times>}, __glibc_reserved1 = {
      0 <repeats 24 times>}}, __ssp = {0, 0, 0, 0}}

      The general register values get changed.

      */

	printf("%s: setcontext_called = %d\n", __FUNCTION__, setcontext_called);
	if (setcontext_called == 1) {
		/* QUESTION: will we get here? why or why not?
         *
         * ANSWER: At the end of main when the thread switch happens.
         *
         * */

		show_interrupt();
		exit(0);
	}

	get_start_end(&start, &end);
	printf("start = 0x%lx\n", start);
	printf("end = 0x%lx\n", end);
	/*
	 * comment out TBD, and replace the -1 in each printf() with what is
	 * expected to be printed.
	 */
	//TBD();

	/* show size of ucontext_t structure. Hint: use sizeof(). */
	printf("ucontext_t size = %ld bytes\n", (long int)sizeof(ucontext_t));

	/* now, look inside of the context you just saved. */

	/* first, think about code */
	/* the program counter is called rip in x86-64 */
	printf("memory address of main() = 0x%lx\n", (unsigned long)(&main));
	printf("memory address of the program counter (RIP) saved "
	       "in mycontext = 0x%lx\n",
	       (unsigned long)mycontext.uc_mcontext.gregs[REG_RIP]);

	/* now, think about parameters */
	printf("argc = %d\n", -1);
	printf("argv = %p\n", (void *)-1);
	/* QUESTIONS: how are these parameters passed into the main function?
	 * are there any saved registers in mycontext that store the parameter
	 * values above. why or why not? Hint: Use gdb, and then run
	 * "disassemble main" in gdb, and then scroll up to see the beginning of
	 * the main function.
     *
     * ANSWER:
     * - These parameters are passed as arguments to the main function.
     * - They are initially saved in the rdi (first argument) and the rsi (second argument) registers.
     * */

	/* now, think about the stack */
	/* QUESTIONS: Is setcontext_called and err stored on the stack? does the
	 * stack grow up or down? What are the stack related data in
	 * mycontext.uc_mcontext.gregs[]?
     *
     * ANSWER:
     *  - According to info locals output:
     * (gdb) info locals
        start = 140737488347792
        end = 93824992235984
        setcontext_called = 0
        err = 0
        __PRETTY_FUNCTION__ = "main"
        __FUNCTION__ = "main"

        the variables are stored on the stack.
     *
     *  - The stack growns downwards, according to the values of start and end (the difference is negative, which means
     *      the addresses reduces).
     *  - REG_RSP stores the stack pointer information.
     *  */
	printf("memory address of the variable setcontext_called = %p\n",
	       (void *)&setcontext_called);
	printf("memory address of the variable err = %p\n",
	       (void *)&err);
    /*sizeof(int) is 4 bytes, so there number of bytes between the variables is 4 as well. So they are stored right next to
     * eachother on the stack. */
	printf("number of bytes pushed to the stack between setcontext_called "
	       "and err = %ld\n", (long int)abs((unsigned long)&setcontext_called - (unsigned long)&err));

	printf("stack pointer register (RSP) stored in mycontext = 0x%lx\n",
	       (unsigned long)mycontext.uc_mcontext.gregs[REG_RSP]);

	printf("number of bytes between err and the saved stack in mycontext "
	       "= %ld\n", (long int)abs((unsigned long)mycontext.uc_mcontext.gregs[REG_RSP] - (unsigned long)&err));

	/* QUESTION: what is the uc_stack field in mycontext? Note that this
	 * field is used to store an alternate stack for use during signal
	 * handling, and is NOT the stack of the running thread.
     *
     * ANSWER:
     * - This is the stack pointer location that we created for the new context.
     *      Additional info: https://people.kth.se/~johanmon/ose/assignments/contexts.pdf
     *          Description: Then we explicitly change the uc_stack elements of the contexts. This is
        a structure holding pointers and size of the stack. We are thus giving each
        context a stack of their own.

        */
	printf("value of uc_stack.ss_sp = 0x%lx\n",
	       (unsigned long)mycontext.uc_stack.ss_sp);

	/* Don't move on to the next part of the lab until you know how to
	 * change the stack in a context when you manipulate a context to create
	 * a new thread. */


	/* now we will try to understand how setcontext works */
	setcontext_called = 1;
	call_setcontext(&mycontext);
	/* QUESTION: why does the program not fail at the assert below?
     *
     * ANSWER: call_setcontext switches context. The result is that we don't return to this context, and we change context and don't
     * hit this line. */
	assert(0);
}

static void
call_setcontext(ucontext_t * context)
{
	int err = setcontext(context);
	assert(!err);
}

static void
show_interrupt(void)
{
	int err;

	/* QUESTION: how did we get here if there was an assert above? */

	/* now think about interrupts. you will need to understand how they
	 * work, and how they interact with get/setcontext for implementing
	 * preemptive threading. */

	/*
	Delete the lines below:

	type = struct ucontext_t {
		unsigned long uc_flags;
		struct ucontext_t *uc_link;
		stack_t uc_stack;
		mcontext_t uc_mcontext;
		sigset_t uc_sigmask;
		struct _libc_fpstate __fpregs_mem;
		unsigned long long __ssp[4];
	}
	*/

	/* QUESTION: what does interrupts_on() do? see interrupt.c */
	interrupts_on();

	/* getcontext stores the process's signal mask */
	err = getcontext(&mycontext);
	assert(!err);

	/* QUESTION: Are interrupts masked (i.e., disabled) in mycontext?
	 * HINT: use sigismember below.
	 *
	 * ANSWER: Interrupts are masked after the interrupts_on() call. */
	printf("interrupt is disabled = %d\n",
	       (unsigned int)sigismember(&mycontext.uc_sigmask, SIG_TYPE));

	interrupts_off();

	err = getcontext(&mycontext);
	assert(!err);

	/* QUESTION: which fields of mycontext changed as a result of the
	 * getcontext call above?
	 *
	 * ANSWER: mycontext.uc_sigmask field changed.
	 * */
	printf("interrupt is disabled = %d\n",
	       (unsigned int)sigismember(&mycontext.uc_sigmask, SIG_TYPE));
}
