#include <stdlib.h>
#include "thread.h"

#define THREAD_SIZE 100

static struct Thread *threads[THREAD_SIZE];

int next_available = 1;
int current = -1;

static void create_thread_internal(
    void (*func)(), int slot, void (*quit_func)());
static void kill();
static int get_next_thread();
static int circular_next(int v);
static void quit_thread();
static void quit_process();
void switch_to(
    void **ebp, void **esp, void ***old_ebp_ptr, void ***old_esp_ptr);

void init_main_thread(void (*func)()) {
  create_thread_internal(func, 0, &quit_process);
}

void create_thread(void (*func)()) {
  create_thread_internal(func, next_available++, &quit_thread);
}

void create_thread_internal(
    void (*func)(), int slot, void (*quit_func)()) {
  struct Thread *new_thread = (struct Thread*) malloc(sizeof(struct Thread));
  new_thread->stack[STACK_SIZE - 1] = NULL;
  new_thread->stack[STACK_SIZE - 2] = quit_func;
  new_thread->stack[STACK_SIZE - 3] = func;
  new_thread->ebp = &new_thread->stack[STACK_SIZE - 1];
  new_thread->esp = &new_thread->stack[STACK_SIZE - 3];
  threads[slot] = new_thread;
}

void yield() {
  int next = get_next_thread();
  if (next == current) {
    return;
  }
  void **ebp = threads[next]->ebp;
  void **esp = threads[next]->esp;
  void ***old_ebp_ptr = NULL;
  void ***old_esp_ptr = NULL; 
  // Save the stack frame of the current thread. There is no current thread 
  // under 2 situations: call from outside thread, e.g. main function, where 
  // current == -1; or current thread is just killed, 
  // where threads[current] == NULL.
  if (current != -1 && threads[current] != NULL) {
    old_ebp_ptr = &(threads[current]->ebp);
    old_esp_ptr = &(threads[current]->esp);
  }
  current = next;
  switch_to(ebp, esp, old_ebp_ptr, old_esp_ptr);
}

void quit_thread() {
  kill();
  yield();
}

void quit_process() {
  kill();
  exit(0);
}

void kill() {
  free(threads[current]);
  threads[current] = NULL;
}

int get_next_thread() {
  int i;  
  for (i = circular_next(current); i != current; i = circular_next(i))
  {
    if (threads[i] != NULL) {
      return i;
    }
  }
  return current;
}

int circular_next(int v) {
  int next = v + 1;
  return next >= THREAD_SIZE ? 0 : next;
}
