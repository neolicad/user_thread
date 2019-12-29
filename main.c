#include <stdio.h>
#include "thread.h"

void A();
void B();
void loop_yield();

int main(void) {
  create_thread(&A);
  create_thread(&B);
  init_main_thread(&loop_yield);
  yield();
  return 0;
}

void A() {
  printf("A before yield!\n");
  yield();
  printf("A after yield!\n");
}

void B() {
  printf("B before yield!\n");
  yield();
  printf("B after yield!\n");
}

void loop_yield() {
  int i;
  for (i = 0; i < 5; i++) {
    printf("main before yield!\n");
    yield();
    printf("main after yield!\n");
  }
}
