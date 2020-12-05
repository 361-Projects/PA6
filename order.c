/********************************************************
 * PROGRAM: User-Level Thread Library                   *
 * CLASS: CISC 361-011                                  *
 * AUTHORS:                                             *
 *    Alex Sederquest | alexsed@udel.edu | 702414270    *
 *    Ben Segal | bensegal@udel.edu | 702425559         *
 ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "ud_thread.h"

sem_t *s1, *s2, *s4;

void *worker1(int pri) {
  sem_wait(s2);
  sem_signal(s1);
  printf("I am worker 1\n");
  t_yield();
  return NULL;
}

void *worker2(int pri) {
  sem_signal(s2);
  printf("I am worker 2\n");
  t_yield();
  return NULL;
}

void *worker3(int pri) {
  sem_wait(s4);
  printf("I am worker 3\n");
  t_yield();
  return NULL;
}

void *worker4(int pri) {
  sem_wait(s1);
  sem_signal(s4);
  printf("I am worker 4\n");
  t_yield();
  return NULL;
}

int main(int argc, char *argv[])
{
  t_init();
  sem_init(&s2, 0);
  sem_init(&s1, 0);
  sem_init(&s4, 0);
  t_create(worker3, 3, 1);
  t_create(worker4, 4, 1);
  t_create(worker1, 1, 1); 
  t_create(worker2, 2, 1);
  t_yield();
  t_yield();
  t_yield();
  t_yield();
  t_yield();
  t_yield();
  t_yield();
  t_yield();
  sem_destroy(&s1);
  sem_destroy(&s2);
  sem_destroy(&s4);
  printf("done.......\n");
  return 0;
}