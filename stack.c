#include "stack.h"

void createStack(Stack *s, int maxDIM){
  s->s = (int *)malloc(sizeof(int) * maxDIM);
  s->top = -1;
  s->dim = maxDIM;

  if(s == NULL){
    exit(1);
  }
}

void addElement(Stack *s, int item){
  if(s->top == s->dim - 1){
    printf("\nerror occurred: overflow\n");
  } else {
    s->top++;
    s->s[s->top] = item;
  }
}

void deleteElement(Stack *s){
  if(s->top == -1) printf("\nerroor occurred: underflow\n");
  else{
    (s->top)--;
    //printf("The removed element was %d", s->s[*s->top + 1]);
  }
}