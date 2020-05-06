#ifndef UNTITLED5_STACK_H
#define UNTITLED5_STACK_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    int *s;
    int *top;
} Stack;

Stack createStack(int max_stack_size);
bool full(int max_stack_size, Stack s);
bool empty(int max_stack_size, Stack s);
void addElement(Stack *s, int item, int max_stack_size);
void deleteElement(Stack *s);
void printStack(Stack s);

#endif //UNTITLED5_STACK_H
