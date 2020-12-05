/********************************************************
 * PROGRAM: User-Level Thread Library                   *
 * CLASS: CISC 361-011                                  *
 * AUTHORS:                                             *
 *    Alex Sederquest | alexsed@udel.edu | 702414270    *
 *    Ben Segal | bensegal@udel.edu | 702425559         *
 ********************************************************/
#include <stdio.h>
#include <unistd.h>
#include "ud_thread.h"

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

sem_t *s1, *s2;

void child_1(int pri) {
    printf("child 1: before\n");
    // what goes here?
    sem_signal(s1);
    t_yield();
    sem_wait(s2);
    printf("child 1: after\n");
    return NULL;
}

void child_2(int pri) {
    printf("child 2: before\n");
    // what goes here?
    sem_signal(s2);
    t_yield();
    sem_wait(s1);
    printf("child 2: after\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    t_init(); // gotta init the lib
    printf("parent: begin\n");
    // init semaphores here
    sem_init(&s1, 0);
    sem_init(&s2, 0);
    t_create(child_1, 1, 1);
    t_create(child_2, 2, 1);
    t_yield();
    t_yield();
    t_yield();
    sem_destroy(&s1);
    sem_destroy(&s2);
    printf("parent: end\n");
    return 0;
}
