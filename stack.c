#include "stack.h"

Stack createStack(int max_stack_size){
    int i;
    Stack s;

    s.s = (int *)malloc(sizeof(int) * max_stack_size);

    *s.top = -1;

    return s;
}

bool full(int max_stack_size, Stack s){
    if(*s.top == max_stack_size - 1) return true;
    else return false;
}

bool empty(int max_stack_size, Stack s){

    if(*s.top < 0) return true;
    else return false;
}

void addElement(Stack *s, int item, int max_stack_size){
    if(*s->top == max_stack_size - 1) printf("\nERROR - OVERFLOW\n");
    else{
        (*s->top)++;
        s->s[*s->top] = item;
    }
}

void deleteElement(Stack *s){
    if(*s->top == -1) printf("\nERROR - UNDERFLOW\n");
    else{
        (*s->top)--;
        //printf("The removed element was %d", s->s[*s->top + 1]);
    }
}

void printStack(Stack s){
    int i;
    if(*s.top >= 0){
        for(i = *s.top; i >= 0; i--){
            printf("%d\n", s.s[i]);
        }
    } else printf("\nERROR - STACK IS EMPTY\n");
}
