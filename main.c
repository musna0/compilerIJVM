#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "stack.h"

#define DIM 128

typedef struct{
  char name[DIM];
  int value;
} Var;

Var v[DIM];
int curr = 0;
Var con[DIM];

FILE * openFile(char*);
bool checkExist(char*);
void simulation(int, char*, Stack*, char*, FILE *);
void identifyInstruction(char*, char*, Stack*, FILE *);
void identifyNumber(char*, char*);
void identifyVar(char*, FILE *);
int identifyLabl(char*, char*, FILE *);

int main(){
  Stack s;
  FILE * fp = NULL;
  char fName[DIM*2];
  int condizione, i;

  createStack(&s, DIM);
  //inizializzo metà stack a 0
  for(i = 0; i < DIM/2; i++){
    addElement(&s, 0);
  }

  printf("Inserire il nome del file da eseguire (compresa estensione)");
  do{
    printf("\n>");
    scanf("%[^\n]", fName);
    if(checkExist(fName)){
      fp = openFile(fName);
      condizione = 1;
    } else {
      printf("Non esiste alcun file chiamato %s\n", fName);
      printf("Inserire nome valido");
      getchar();
      condizione = 0;
    }
  } while(!condizione);
  
  simulation(0, " ", &s, fName, fp);
  

  fclose(fp);
  return 0;
}

FILE * openFile(char *fName){
  FILE * fp = NULL;
  
  fp = fopen(fName, "r");
  if(fp == NULL) {
    printf("error occurred: failed to open file - exiting");
    exit(-1);
  }

  return fp;
}

bool checkExist(char *fName){
    if( access( fName, F_OK ) != -1 ) return true;
    else return false;
}

void simulation(int mode, char *label, Stack *s, char* fName, FILE * fp){
  char line[DIM*2];
  
  while(fgets(line, sizeof(line), fp)){
    if(mode == 1){
      int res = identifyLabl(label, fName, fp);
        if(res == 1){
            simulation(0, " ", s, fName, fp);
        }
        if(res == -1){
          printf("\nError occurred - no label found");
        }
    }
    else identifyInstruction(line, fName, s, fp);
  }
}

void identifyInstruction(char* line, char* fName, Stack *s, FILE * fp){
  char word[DIM*2] = " ", number[DIM*2] = " ";
  int i, firstChara = 0, w = 0;

  for(i = 0; i < strlen(line); i++){
    if(line[i] != ' ' && line[i] != '\n') firstChara = 1;
    if(firstChara == 1){
      if(line[i] != ' ' && line[i] != '\n'){
        word[w] = line[i];
        w++;
      } else i = strlen(line);
    }
  }
  
  if(strcmp(word, ".var") == 0){
        identifyVar(fName, fp);
  }
  else if(strcmp(word, "BIPUSH") == 0|| strcmp(word, "bipush") == 0){
    identifyNumber(line, number);
    addElement(s, atoi(number));
  } else if(strcmp(word, "DUP") == 0 || strcmp(word, "dup") == 0){
    addElement(s, s->s[s->top]);
  } else if(strcmp(word, "ERR") == 0 || strcmp(word, "err") == 0){
    printf("\nerror occurred - exiting simulation");
    exit(-1);
  } else if(strcmp(word, "GOTO") == 0 || strcmp(word, "goto") == 0){
    char label[DIM], m = 0;
      for(int x = w + 1; x < (int)strlen(line) + 1; x++) {
        if (line[i] != ' ') {
          label[x - (w + 1)] = line[x];
            m++;
          }
        }
    char temp, temp1;
    temp = label[m - 2];
    temp1 = label[m - 1];
    label[m - 2] = ':';
    label[m - 1] = temp;
    label[m] = temp1;

    simulation(1, label, s, fName, fp);
  } else if(strcmp(word, "HALT") == 0 || strcmp(word, "halt") == 0){
    printf("\nexiting simulation with no errors");
    exit(0);
  } else if(strcmp(word, "IADD") == 0 || strcmp(word, "iadd") == 0){
    int sum = s->s[s->top] + s->s[s->top - 1];
    deleteElement(s);
    deleteElement(s);
    addElement(s, sum);
  } else if(strcmp(word, "ISUB") == 0 || strcmp(word, "isub") == 0){
    int sub = s->s[s->top - 1] - s->s[s->top];
    deleteElement(s);
    deleteElement(s);
    addElement(s, sub);
  } else if(strcmp(word, "IAND") == 0 || strcmp(word, "iand") == 0){
    int and = s->s[s->top] & s->s[s->top - 1];
    deleteElement(s);
    deleteElement(s);
    addElement(s, and);
  } else if(strcmp(word, "IOR") == 0 || strcmp(word, "ior") == 0){
    int or = s->s[s->top] | s->s[s->top - 1];
    deleteElement(s);
    deleteElement(s);
    addElement(s, or);
  } else if(strcmp(word, "IFEQ") == 0 || strcmp(word, "ifeq") == 0) {
    if(s->s[s->top] == 0){
            deleteElement(s);
            printf("IFEQ");
            char label[DIM], m = 0;
            for(int x = w + 1; x < (int)strlen(line) + 1; x++) {
                if (line[i] != ' ') {
                    label[x - (w + 1)] = line[x];
                    m++;
                }
            }
            char temp, temp1;
            temp = label[m - 2];
            temp1 = label[m - 1];
            label[m - 2] = ':';
            label[m - 1] = temp;
            label[m] = temp1;

            simulation(1, label, s, fName, fp);
        }
  } else if(strcmp(word, "IFLT") == 0 || strcmp(word, "iflt") == 0) {
    if(s->s[s->top] < 0){
            deleteElement(s);
            char label[DIM], m = 0;
            for(int x = w + 1; x < (int)strlen(line) + 1; x++) {
                if (line[i] != ' ') {
                    label[x - (w + 1)] = line[x];
                    m++;
                }
            }
            char temp, temp1;
            temp = label[m - 2];
            temp1 = label[m - 1];
            label[m - 2] = ':';
            label[m - 1] = temp;
            label[m] = temp1;

            simulation(1, label, s, fName, fp);
        }
  } else if(strcmp(word, "IF_ICMPEQ") == 0 || strcmp(word, "if_icmpeq") == 0) {
    if(s->s[s->top] == s->s[s->top - 1]){
            deleteElement(s);
            deleteElement(s);
            printf("IF_ICMPEQ");
            char label[DIM], m = 0;
            for(int x = w + 1; x < (int)strlen(line) + 1; x++) {
                if (line[i] != ' ') {
                    label[x - (w + 1)] = line[x];
                    m++;
                }
            }
            char temp, temp1;
            temp = label[m - 2];
            temp1 = label[m - 1];
            label[m - 2] = ':';
            label[m - 1] = temp;
            label[m] = temp1;

            simulation(1, label, s, fName, fp);
        }
  } else if(strcmp(word, "IINC") == 0 || strcmp(word, "iinc") == 0){
      char temp[DIM], num[DIM];
      int x = w + 1;

      for(i = w + 1; i < (int)strlen(line) + 1; i++){
        if(line[i] != ' '){
          temp[i - (w + 1)] = line[i];
          x++;
        }
      }
      for(i = x - 2; i < (int)strlen(line); i++){
        if(line[i] != ' '){
          num[i - (x - 2)] = line[i];
        }
      }
      for(i = 0; i < DIM; i++){
        if(strcmp(temp, v[i].name) == 0){
          v[i].value += atoi(num);
          }
      }
  } else if(strcmp(word, "ILOAD") == 0 || strcmp(word, "iload") == 0){
      char temp[DIM];

      for(i = w + 1; i < (int)strlen(line) + 1; i++){
        if(line[i] != ' '){
            temp[i - (w + 1)] = line[i];
        }
      }
      for(i = 0; i < DIM; i++){
        if(strcmp(temp, v[i].name) == 0){
          addElement(s, v[i].value);
        }
      }
  } else if(strcmp(word, "IN") == 0 || strcmp(word, "in") == 0){
        //input
  } else if(strcmp(word, "INVOKEVIRTUAL") == 0 || strcmp(word, "invokevirtual") == 0){
        //jump to method
  } else if(strcmp(word, "IRETURN") == 0 || strcmp(word, "ireturn") == 0){
        //return method value
  } else if(strcmp(word, "ISTORE") == 0 || strcmp(word, "istore") == 0){
        char temp[DIM];

        for(i = w + 1; i < (int)strlen(line) + 1; i++){
            if(line[i] != ' '){
                temp[i - (w + 1)] = line[i];
            }
        }
        for(i = 0; i < DIM; i++){
            if(strcmp(temp, v[i].name) == 0){
                v[i].value = s->s[s->top];
                deleteElement(s);
            }
        }
    } else if(strcmp(word, "LDC_W") == 0 || strcmp(word, "ldc_w") == 0){
        char temp[DIM];
        int j = 0;
        for(i = w + 1; i < (int)strlen(line) + 1; i++){
          if(line[i] != ' '){
            temp[i - (w + 1)] = line[i];
              j++;
          }
        }
        for(i = 0; i < DIM; i++){
          if(strcmp(temp, con[i].name) == 0){
              addElement(s, con[i].value);
          }
        }
    } else if(strcmp(word, "NOP") == 0 || strcmp(word, "nop") == 0){

    } else if(strcmp(word, "OUT") == 0 || strcmp(word, "out") == 0){
      printf("\n%d", s->s[s->top]);
    } else if(strcmp(word, "POP") == 0 || strcmp(word, "pop") == 0){
      deleteElement(s);
    } else if(strcmp(word, "SWAP") == 0 || strcmp(word, "swap") == 0){
        int temp;
        temp = s->s[s->top];
        s->s[s->top] = s->s[s->top - 1];
        s->s[s->top - 1] = temp;
    }
}


void identifyNumber(char *line, char *number){
  int i, x = 0;

  for(i = 0; i < strlen(line); i++){
    if(line[i] == '1' || line[i] == '2'|| line[i] == '3' || line[i] == '4'|| line[i] == '5'|| line[i] == '6'|| line[i] == '7' || line[i] == '8'|| line[i] == '9' || line[i] == '0'){
        number[x] = line[i];
        x++;
      }
  }
}

void identifyVar(char* fName, FILE * fp){
    char line[DIM];
    int start = 0, end = 0;

    fclose(fp);
    fp = openFile(fName);
    while(fgets(line, sizeof(line), fp)) {
        if(strcmp(line, ".end-var\n") == 0) end = 1;
        if(start == 1 && end != 1){
            strcpy(v[curr].name, line);
            //la riga successiva stampa i nomi delle variabili se non è commentata
            //printf("%s", v[curr].name);
            v[curr].value = 0;
            curr++;
        }
        else if(end == 1){
            break;
        }
        if(strcmp(line, ".var\n") == 0) start = 1;
    }
}

int identifyLabl(char *label, char* fName, FILE * fp){
    char line[DIM];

    fclose(fp);
    fp = openFile(fName);
    while(fgets(line, sizeof(line), fp)) {
        if(strcmp(line, label) == 0) return 1;
    }
    return -1;
}