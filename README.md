# user_thread
A 32bit program for creating user threads.

## Usage
```c
int main(void) {
  ...
  init_main_thread(&loop_yield);
  create_thread(&A);
  create_thread(&B);
  yield();
  ...
}
```

## Bug
We just increment the index to assign the thread slot. This is wrong. Fix it later.

## Model
After the first yield(), we will jump across threads (we will never come back to the inialization main function). 
There is a main thread which just continuously yield and will exit the process when return. Other threads execute
normal functions and will be cleaned up when return.

## Limitation
* 4k fixed stack size
* Round robin rotation searching throughout the whole list

## Notice
* When killing the thread, we first free the thread struct (thus the stack) and then call yield(). This means 
yield() will execute in the freed space. This should be fine because the freed space is accessed exclusively by the 
current thread. The reason is that there could only be 1 user thread executing at any given time, and the memory 
space is private so other processes cannot access.
