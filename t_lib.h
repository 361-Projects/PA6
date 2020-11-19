

/********************************
 *          INCLUDES            *
 ********************************/
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/mman.h>

/********************************
 *          Type Defs           *
 ********************************/
struct tcb {
    int thread_id;
    int thread_priority;
    ucontext_t *thread_context;
    struct tcb *next;
};

typedef struct tcb tcb;