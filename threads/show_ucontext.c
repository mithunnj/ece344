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

static void call_setcontext(ucontext_t * context);
static void show_interrupt(void);

/* zero out the context */
ucontext_t mycontext = { 0 };

void
get_start_end(long *startp, long *endp)
{
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
     * ANSWER: NOTE Left off here set */
	volatile int setcontext_called = 0;
	int err;

	/*
	 * DO NOT CHANGE/ADD ANY CODE UNTIL BELOW TBD(). SEE BELOW.
	 */

	/* Get context: make sure to read the man page of getcontext in detail,
	 * or else you will not be able to answer the questions below.
     *
     * Additional info: getcontext initializes the structure pointed that is passed in (mycontext) to the current active context.
     * Returns 0 on success, -1 on errno.*/
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
	 * - Use "ptype mycontext" so see the type/fields of mycontext i
     *
     * ANSWER: The following fields of mycontext changed:
         *  type = struct ucontext_t {
            unsigned long uc_flags;         -> NO CHANGE
            struct ucontext_t *uc_link;     -> NO CHANGE
            stack_t uc_stack;               -> NO CHANGE
            mcontext_t uc_mcontext;         -> CHANGE
            sigset_t uc_sigmask;            -> CHANGE
            struct _libc_fpstate __fpregs_mem; -> CHANGE
            unsigned long long __ssp[4]; -> NO CHANGE
        }

        Additional notes:
            - mcontext_t contains the actual state of the proccess.
            - more info for the ucontext_t struct can be found here: https://www.gnu.org/software/libc/manual/html_node/System-V-contexts.html

        Specific changes:
        1. uc_mcontext:
            BEFORE:
                uc_mcontext = {gregs = {0 <repeats 23 times>}, fpregs = 0x0, __reserved1 = {0, 0, 0, 0, 0, 0, 0, 0}}
            AFTER:
                uc_mcontext = {gregs = {140737353751936, 140737353751936, 0, 0, 93824992235984, 140737488349648, 0, 0, 93824992264480,
      140737488349656, 140737488349424, 0, 140737488349672, 0, 140737353746200, 140737488349376, 93824992236462, 0, 0, 0, 0, 0, 0}, fpregs = 0x55555555c2c8 <mycontext+424>, __reserved1 = {0, 0, 0, 0, 0, 0, 0,
      0}}
        2. uc_sigmask:
            BEFORE:
                uc_sigmask = {
    __val = {0 <repeats 16 times>}}, __fpregs_mem = {cwd = 0, swd = 0, ftw = 0, fop = 0, rip = 0, rdp = 0, mxcsr = 0, mxcr_mask = 0, _st = {{significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0,
          0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0,
        __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0,
          0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}}, _xmm = {{element = {0, 0, 0, 0}} <repeats 16 times>},
    __glibc_reserved1 = {0 <repeats 24 times>}}, __ssp = {0, 0, 0, 0}
        AFTER:
            uc_sigmask = {__val = {0 <repeats 16 times>}}, __fpregs_mem = {cwd = 895, swd = 65535, ftw = 0, fop = 65535, rip = 4294967295, rdp = 0, mxcsr = 8064, mxcr_mask = 0, _st = {{significand = {0, 0, 0,
          0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {
        significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0,
        __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}, {significand = {0, 0, 0, 0}, exponent = 0, __glibc_reserved1 = {0, 0, 0}}}, _xmm = {{element = {
          0, 0, 0, 0}} <repeats 16 times>}, __glibc_reserved1 = {0 <repeats 24 times>}}, __ssp = {0, 0, 0, 0}

     *  */

	printf("%s: setcontext_called = %d\n", __FUNCTION__, setcontext_called);
	if (setcontext_called == 1) {
		/* QUESTION: will be get here? why or why not?
         *
         * ANSWER: We will never enter this because the value of setcontext_called was initialized as 0, and the value does not get
         * modified. */

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

	/* show size of ucontext_t structure. Hint: use sizeof(). */
	printf("ucontext_t size = %ld bytes\n", (long int)sizeof(ucontext_t));

	/* now, look inside of the context you just saved. */

	/* first, think about code */
	/* the program counter is called rip in x86-64 */
	printf("memory address of main() = 0x%lx\n", (unsigned long)&main);
    /* NOTE: The location of each register in the general register can be found in the gregset_t enum description in
     * /usr/include/sys/ucontext.h */
	printf("memory address of the program counter (RIP) saved "
	       "in mycontext = 0x%lx\n",
	       (unsigned long)mycontext.uc_mcontext.gregs[REG_RIP]);

	/* now, think about parameters */
	printf("argc = %d\n", argc);
	printf("argv = %p\n", (void *)argv);
	/* QUESTIONS: how are these parameters passed into the main function?
	 * are there any saved registers in mycontext that store the parameter
	 * values above. why or why not? Hint: Use gdb, and then run
	 * "disassemble main" in gdb, and then scroll up to see the beginning of
	 * the main function.
     *
     * ANSWER:
     *  - NOTE: Need to answer if the parameters are stored in the disassemble main call - still unsure on how to read the GDB dump
     *  information.
     *  */


	/* now, think about the stack */
	/* QUESTIONS: Is setcontext_called and err stored on the stack? does the
	 * stack grow up or down? What are the stack related data in
	 * mycontext.uc_mcontext.gregs[]?
     *
     * ANSWERS:
     *  - NOTE: Need to answer the following: Is setcontext_called and err stored on the stack?
     *  - The stack grows down, that's why in the GDB logs, you keep seeing memory locations being substracted.
     *  - As described here: https://sites.uclouvain.be/SystInfo/usr/include/sys/ucontext.h.html, the stack related data in
     *  mycontext.uc_mcontext.gregs[] contains the general register locations that will contain information like the program
     *  counter, stack pointer, argument registers, etc. */
	printf("memory address of the variable setcontext_called = %p\n",
	       (void *)&setcontext_called);
	printf("memory address of the variable err = %p\n",
	       (void *)&err);
	printf("number of bytes pushed to the stack between setcontext_called "
	       "and err = %ld\n", (unsigned long)(abs(&setcontext_called - &err)));

	printf("stack pointer register (RSP) stored in mycontext = 0x%lx\n",
	       (unsigned long)mycontext.uc_mcontext.gregs[REG_RSP]);

	printf("number of bytes between err and the saved stack in mycontext "
	       "= %ld\n", (unsigned long)abs(&mycontext.uc_mcontext.gregs[REG_RSP] - &mycontext.uc_mcontext.gregs[REG_ERR]));

	/* QUESTION: what is the uc_stack field in mycontext? Note that this
	 * field is used to store an alternate stack for use during signal
	 * handling, and is NOT the stack of the running thread.
     *
     * ANSWER:
     *  - stack_t uc_stack: Stack used for this context. The value need not be (and normally is not) the stack pointer.i A signal stack is a special area of memory to be used as the execution stack during signal handlers.
     *  Additinal info:
     *      - https://www.gnu.org/software/libc/manual/html_node/Signal-Stack.html
     *      - https://www.gnu.org/software/libc/manual/html_node/System-V-contexts.html */
	printf("value of uc_stack.ss_sp = 0x%lx\n",
	       (unsigned long)mycontext.uc_stack.ss_sp);

	/* Don't move on to the next part of the lab until you know how to
	 * change the stack in a context when you manipulate a context to create
	 * a new thread.
     *
     * ANSWER: We use the setcontext call to change the context to the one pointed to by the given argument */

	/* now we will try to understand how setcontext works */
	setcontext_called = 1;
	call_setcontext(&mycontext);
	/* QUESTION: why does the program not fail at the assert below?
     *
     * ANSWER: As described in the man pages, if successful (0) - setcontext() does not return. It will transfer control to the
     * context passed in (mycontext), and will continue execution from the point at which the context was stored in mycontext.
     * */
	printf("DEBUG: Returned here after the setcontext() switch\n"); // DEBUG REMOVE
	assert(0);
}

static void
call_setcontext(ucontext_t * context)
{
	printf("DEBUG: Verify that change was implemented\n"); // DEBUG REMOVE
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

	/* QUESTION: what does interrupts_on() do? see interrupt.c
     *
     * ANSWER: Enables interrupts.*/
	interrupts_on();

	/* getcontext stores the process's signal mask */
	err = getcontext(&mycontext);
	assert(!err);

	/* QUESTION: Are interrupts masked (i.e., disabled) in mycontext?
	 * HINT: use sigismember below. */
	printf("interrupt is disabled = %d\n",
	       (unsigned int)sigismember(&mycontext.uc_sigmask, SIG_TYPE));

	interrupts_off();

	err = getcontext(&mycontext);
	assert(!err);

	/* QUESTION: which fields of mycontext changed as a result of the
	 * getcontext call above? */
	printf("interrupt is disabled = %d\n",
	       (unsigned int)sigismember(&mycontext.uc_sigmask, SIG_TYPE));
}
