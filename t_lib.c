#include "t_lib.h"

tcb *running;
tcb *ready;

void t_yield() {
  // Make sure we don't yield in the main thread, bad juju ensues
  if (ready != NULL) {
    tcb *yielding, *yieldTo, *tmp;
    yielding = running;
    yieldTo = ready;
    tmp = ready;
    if (tmp != NULL)
      while(tmp->next != NULL) tmp = tmp->next;  
    
    ready = ready->next;
    yieldTo->next = NULL;
    tmp->next = yielding;

    swapcontext(yielding->thread_context, yieldTo->thread_context);

  }
}
/* t_init() initializes the 2 tcb queues with the main context
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

int t_create(void (*fct)(int), int id, int pri) {
  size_t sz = 0x10000;

  ucontext_t *uc = (ucontext_t *)malloc(sizeof(ucontext_t));
  getcontext(uc);

  uc->uc_stack.ss_sp = malloc(sz);
  uc->uc_stack.ss_size = sz;
  uc->uc_stack.ss_flags = 0;
  uc->uc_link = running->thread_context;
  makecontext(uc, (void (*)(void))fct, 1, id);

  tcb *tmp, *new = (tcb *)malloc(sizeof(tcb));
  new->thread_context = uc;
  new->thread_id = id;
  new->thread_priority = pri;
  new->next = NULL;

  tmp = ready;
  if (tmp != NULL) {
    while(tmp->next != NULL) tmp = tmp->next;  
    tmp->next = new;
  } else {
    ready = new;
  }


  // ready = new;
}

void t_terminate() {
  free(running->thread_context->uc_stack.ss_sp);
  free(running->thread_context);
  free(running);
  running = ready;
  ready = ready->next;
  running->next = NULL;
  setcontext(running->thread_context);
}

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
