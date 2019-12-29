#define STACK_SIZE 1000

struct Thread {
  void *stack[STACK_SIZE];
  void **esp;
  void **ebp;
};

void create_thread(void (*func)());
void init_main_thread(void (*func)());
void yield();
