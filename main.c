#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "stack.h"

#define DIM 128

Stack stack;

typedef struct{
    char name[DIM];
    int value;
} Var;

Var v[DIM];
int curr = 0;
Var con[DIM];

FILE * openFile(char*);
void closeFile(FILE *);
bool checkExist(char*);
void identifyInstruction(const char*, FILE *);
void strtoint(const char*, char *);
void simulation(int mode, char*, FILE *);
int identifyLabl(char*, FILE *);
void identifyVar(FILE *);

int main() {
    FILE * fp = NULL;
    stack = createStack(DIM);


    for(int i = 0; i < DIM/2; i++){
        addElement(&stack, 0, DIM);
    }

    simulation(2, " ", fp);

    closeFile(fp);
    return 0;
}

FILE * openFile(char* fName){
    FILE * fp = NULL;

    if(checkExist(fName)){
        fp = fopen(fName, "r");
        if(fp == NULL) exit(-1);
    }else{
        fp = fopen(fName, "w");
        if(fp == NULL) exit(-1);
    }

    return fp;
}

void closeFile(FILE * fp){
    fclose(fp);
}

bool checkExist(char* fName){
    if( access( fName, F_OK ) != -1 ) return true;
    else return false;
}

void identifyInstruction(const char* line, FILE * fp){
    char word[DIM] = "", number[DIM] = "";
    int i, w = 0;

    for(i = 0; i < DIM; i++){
        if(line[i] != ' ' && line[i] != '\n'){
            word[w] = line[i];
            w++;
        }else i = DIM - 1;
    }
    if(strcmp(word, ".var") == 0){
        identifyVar(fp);
    }
    else if(strcmp(word, "BIPUSH") == 0 || strcmp(word, "bipush") == 0){
        strtoint(line, number);
        addElement(&stack, atoi(number), DIM);
    }
    else if(strcmp(word, "DUP") == 0 || strcmp(word, "dup") == 0){
        addElement(&stack, stack.s[0], DIM);
    }
    else if(strcmp(word, "ERR") == 0 || strcmp(word, "err") == 0){
        printf("\nERROR - exiting simulation");
        exit(-1);
    }
    else if(strcmp(word, "GOTO") == 0 || strcmp(word, "goto") == 0){
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

        simulation(1, label, fp);
    }
    else if(strcmp(word, "HALT") == 0 || strcmp(word, "halt") == 0){
        printf("\nexiting simulation with no errors");
        exit(0);
    }
    else if(strcmp(word, "IADD") == 0 || strcmp(word, "iadd") == 0){
        int sum = stack.s[*stack.top] + stack.s[*stack.top - 1];
        deleteElement(&stack);
        deleteElement(&stack);
        addElement(&stack, sum, DIM);
    }
    else if(strcmp(word, "ISUB") == 0 || strcmp(word, "isub") == 0){
        int sub = stack.s[*stack.top - 1] - stack.s[*stack.top];
        deleteElement(&stack);
        deleteElement(&stack);
        addElement(&stack, sub, DIM);
    }
    else if(strcmp(word, "IAND") == 0 || strcmp(word, "iand") == 0){
        int and = stack.s[*stack.top] & stack.s[*stack.top - 1];
        deleteElement(&stack);
        deleteElement(&stack);
        addElement(&stack, and, DIM);
    }
    else if(strcmp(word, "IFEQ") == 0 || strcmp(word, "ifeq") == 0){
        if(stack.s[*stack.top] == 0){
            deleteElement(&stack);
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

            simulation(1, label, fp);
        }
    }
    else if(strcmp(word, "IFLT") == 0 || strcmp(word, "iflt") == 0){
        if(stack.s[*stack.top] < 0){
            deleteElement(&stack);
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

            simulation(1, label, fp);
        }
    }
    else if(strcmp(word, "IF_ICMPEQ") == 0 || strcmp(word, "if_icmpeq") == 0){
        if(stack.s[*stack.top] == stack.s[*stack.top - 1]){
            deleteElement(&stack);
            deleteElement(&stack);
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

            simulation(1, label, fp);
        }
    }
    else if(strcmp(word, "IINC") == 0 || strcmp(word, "iinc") == 0){
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
    }
    else if(strcmp(word, "ILOAD") == 0 || strcmp(word, "iload") == 0){
        char temp[DIM], num[DIM];
        for(i = w + 1; i < (int)strlen(line) + 1; i++){
            if(line[i] != ' '){
                temp[i - (w + 1)] = line[i];
            }
        }
        for(i = 0; i < DIM; i++){
            if(strcmp(temp, v[i].name) == 0){
                addElement(&stack, (int)v[i].value, DIM);
            }
        }
    }
    else if(strcmp(word, "IN") == 0 || strcmp(word, "in") == 0){
        //input
    }
    else if(strcmp(word, "INVOKEVIRTUAL") == 0 || strcmp(word, "invokevirtual") == 0){
        //jump to method
    }
    else if(strcmp(word, "IOR") == 0 || strcmp(word, "ior") == 0){
        int or = stack.s[*stack.top] | stack.s[*stack.top - 1];
        deleteElement(&stack);
        deleteElement(&stack);
        addElement(&stack, or, DIM);
    }
    else if(strcmp(word, "IRETURN") == 0 || strcmp(word, "ireturn") == 0){
        //return method value
    }
    else if(strcmp(word, "ISTORE") == 0 || strcmp(word, "istore") == 0){
        char temp[DIM];
        for(i = w + 1; i < (int)strlen(line) + 1; i++){
            if(line[i] != ' '){
                temp[i - (w + 1)] = line[i];
            }
        }
        for(i = 0; i < DIM; i++){
            if(strcmp(temp, v[i].name) == 0){
                v[i].value = stack.s[*stack.top];
                deleteElement(&stack);
            }
        }
    }
    else if(strcmp(word, "IADD") == 0 || strcmp(word, "iadd") == 0){
        int sub = stack.s[*stack.top - 1] - stack.s[*stack.top];
        deleteElement(&stack);
        deleteElement(&stack);
        addElement(&stack, sub, DIM);
    }
    else if(strcmp(word, "LDC_W") == 0 || strcmp(word, "ldc_w") == 0){
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
                addElement(&stack, (int)con[i].value, DIM);
            }
        }
    }
    else if(strcmp(word, "NOP") == 0 || strcmp(word, "nop") == 0){

    }
    else if(strcmp(word, "OUT") == 0 || strcmp(word, "out") == 0){
        printf("\n%d", stack.s[*stack.top]);
    }
    else if(strcmp(word, "POP") == 0 || strcmp(word, "pop") == 0){
        deleteElement(&stack);
    }
    else if(strcmp(word, "SWAP") == 0 || strcmp(word, "swap") == 0){
        int temp;
        temp = stack.s[*stack.top];
        stack.s[*stack.top] = stack.s[*stack.top - 1];
        stack.s[*stack.top - 1] = temp;
    }

}

void strtoint(const char* line, char* number){
    int x = 0;

    for(int i = 0; i < (int)strlen(line); i++){
        if(line[i] == '1' || line[i] == '2'|| line[i] == '3' || line[i] == '4'|| line[i] == '5'||
           line[i] == '6'|| line[i] == '7' || line[i] == '8'|| line[i] == '9' || line[i] == '0'){
            number[x] = line[i];
            x++;
        }
    }
}


void simulation(int mode, char *search, FILE * fp){
    char * line = NULL;
    size_t len = 0;

    if(mode == 2){
        int  condizione;
        char nomeFile[DIM] = "%&NO_file_NAME&%";
        printf("Inserire il nome del file da aprire, compresa l'estensione.\n"
               "Se si vuole aprire un file in una cartella diversa da quella corrente inserire anche il percorso.");
        do{
            printf("\n>");
            scanf("%[^\n]", nomeFile);
            if(checkExist(nomeFile) == true){
                condizione = 1;
                fp = openFile(nomeFile);
            } else {
                printf("Non esiste un file chiamato %s nella directory corrente, inserire nome valido", nomeFile);
                getchar();
                condizione = 0;
            }
        }while (!condizione);

    }

    while(getline(&line, &len, fp) != -1) {
        if(mode == 1){
            int res = identifyLabl(search, fp);
            if(res == 1){
                simulation(0, " ", fp);
            }
            if(res == -1){
                printf("\nerror occurred - no label found");
            }
        }
        else identifyInstruction(line, fp);
    }
}

int identifyLabl(char *label, FILE * fp){
    char * line = NULL;
    size_t len = 0;

    closeFile(fp);
    fp = openFile("test.txt");
    while(getline(&line, &len, fp) != -1) {
        if(strcmp(line, label) == 0) return 1;
    }
    return -1;
}

void identifyVar(FILE * fp){
    char * line = NULL;
    size_t len = 0;
    int start = 0, end = 0;

    closeFile(fp);
    fp = openFile("test.txt");
    while(getline(&line, &len, fp) != -1) {
        if(strcmp(line, ".end-var\n") == 0) end = 1;
        if(start == 1 && end != 1){
            line[len - 1] = ' ';
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
