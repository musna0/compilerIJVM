#include <stdio.h>
#include <stdlib.h>


typedef struct{
  int *s;
  int top;
  int dim;
} Stack;

void createStack(Stack*, int);
void addElement(Stack*, int);
void deleteElement(Stack*);