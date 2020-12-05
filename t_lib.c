/********************************************************
 * PROGRAM: User-Level Thread Library                   *
 * CLASS: CISC 361-011                                  *
 * AUTHORS:                                             *
 *    Alex Sederquest | alexsed@udel.edu | 702414270    *
 *    Ben Segal | bensegal@udel.edu | 702425559         *
 ********************************************************/

#include "t_lib.h"

tcb *running;
tcb *ready;

/**
 *  t_yield() has the current thread relinquish cpu control to the next thread in the ready queue and
 *           then is put at the end of the ready queue
 *
 * @param Nothing
 * @return Nothing
 */
void t_yield() {
  // Make sure we don't yield in the main thread, bad juju ensues
  if (ready) {
    tcb *yielding, *yieldTo, **tmp = &ready;
    yielding = running;
    yieldTo = ready;
    while(*tmp) tmp = &(*tmp)->next;
    *tmp = yielding;
    ready = ready->next;
    yieldTo->next = NULL;
    running = yieldTo;
    swapcontext(yielding->thread_context, yieldTo->thread_context);
  }
}

/**
 * t_init() initializes the 2 tcb queues with the main context
 * 
 * @param Nothing
 * @return Nothing
 */
void t_init() {
  tcb *head = (tcb *)malloc(sizeof(tcb));
  ucontext_t *context = (ucontext_t *)malloc(sizeof(ucontext_t));

  getcontext(context);

  head->thread_context = context;
  head->thread_id = 0;
  head->thread_priority = 0;  
  head->next = NULL;

  running = head;
  ready = NULL;
}

/**
 *  t_create() Create a new thread
 * 
 * @param fct -> void(*fct)(int), the start funuction
 * @param id -> int, the thread ID
 * @param pri -> int, the thread priority
 * @return Nothing
 */
int t_create(void (*fct)(int), int id, int pri) {
  size_t sz = 0x10000;

  ucontext_t *uc = (ucontext_t *)malloc(sizeof(ucontext_t)); //This bad boi dont get free
  getcontext(uc);

  uc->uc_stack.ss_sp = malloc(sz);
  uc->uc_stack.ss_size = sz;
  uc->uc_stack.ss_flags = 0;
  uc->uc_link = running->thread_context;
  makecontext(uc, (void (*)(void))fct, 1, id);

  tcb *new = (tcb *)malloc(sizeof(tcb));
  new->thread_context = uc;
  new->thread_id = id;
  new->thread_priority = pri;
  new->next = NULL;

  tcb *tmp = ready;
  if (tmp != NULL) {
    while(tmp->next != NULL) tmp = tmp->next;  
    tmp->next = new;
  } else {
    ready = new;
  }

  // ready = new;
}

/**
 *  t_terminate() terminates the currently running thread via its context and sets up the next thread to run
 *
 * @param Nothing
 * @return Nothing
 */
void t_terminate() {
  free(running->thread_context->uc_stack.ss_sp);
  free(running->thread_context);
  free(running);
  running = ready;
  if (ready != NULL) {
    ready = ready->next;
  }
  if (running != NULL) {
    running->next = NULL;
    setcontext(running->thread_context);
  }
}

/**
 *  t_shutdown() frees the ready and running queue so there are no memory leaks
 *
 * 
 * @param Nothing
 * @return Nothing
 */
void t_shutdown() {
  // Clear/free memory of the ready queue
  tcb *tmp = ready;
  while(ready != NULL) {
    tmp = ready->next;
    free(ready->thread_context->uc_stack.ss_sp);
    free(ready->thread_context);
    free(ready);
    ready = tmp;
  }
  
  // Clear/free memory of the running queue
  tmp = running;
  while (running != NULL) {
    tmp = running->next;
    free(running->thread_context->uc_stack.ss_sp);
    free(running->thread_context);
    free(running);
    running = tmp;
  }
}

/**
 * sem_init() Init a semaphore
 *
 * @param sp -> sem_t**, the semaphore to be pinted to
 * @param sem_count -> int, the count of the semaphore
 * @return sem_count
 */
int sem_init(sem_t **sp, int sem_count)
{
  *sp = malloc(sizeof(sem_t));
  (*sp)->count = sem_count;
  (*sp)->q = NULL;
  return sem_count;
}

/**
 * sem_wait() waits for a specified semaphore
 *
 * @param s -> sem_t*, The semaphore to be waited for
 * @return Nothing
 */
void sem_wait(sem_t *s) {
    sighold();
    s->count--; // p
    if (s->count < 0) {
        tcb *currently_running = running, **tmp;
        running = ready;
        ready = ready->next;
        tmp = &(s->q);
        while (*tmp) tmp = &(*tmp)->next;
        *tmp = currently_running;
        running->next = NULL;
        swapcontext(currently_running->thread_context, running->thread_context);
        sigrelse();
    } else 
        sigrelse();
}


/**
 * sem_signal, the thread that signals becomes ready and the first thread in the q becomes ready
 * 
 * @param s -> sem_t, the semaphore
 * @return Nothing
 */
void sem_signal(sem_t *s) {
    sighold();
    s->count++; // v
    if (s->count <= 0) {
        tcb **tmp = &ready, *semTemp = s->q;
        s->q = s->q->next;
        semTemp->next = NULL;
        while (*tmp) tmp = &(*tmp)->next;
        *tmp = semTemp;
        sigrelse();
    } else 
        sigrelse();
  } 

/**
 * sem_destroy() Yeet a semaphore to the memory gods in memhalla
 * 
 * @param s -> sem_t**, The pointer to the semaphore to be deleted
 * @return Nothing
 */
void sem_destroy(sem_t **s)
{
  free(*s);
}
