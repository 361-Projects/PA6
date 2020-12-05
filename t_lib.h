/********************************************************
 * PROGRAM: User-Level Thread Library                   *
 * CLASS: CISC 361-011                                  *
 * AUTHORS:                                             *
 *    Alex Sederquest | alexsed@udel.edu | 702414270    *
 *    Ben Segal | bensegal@udel.edu | 702425559         *
 ********************************************************/

/********************************
 *          INCLUDES            *
 ********************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
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

typedef struct {
  int count;
  tcb *q;
} sem_t;
