/*
 * mmn14.h
 *
 *  Created on: May 31, 2022
 *      Author: user
 */

#ifndef MMN14_H_
#define MMN14_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
typedef int REGISTER[10];
typedef int WORD[10];
WORD memory[256];
int PC;
char* readline();
char* preasembler(FILE* f);
int ignorewhitechar(char* line,int ind);
int compareignore(char* s1, char* s2);
int indexof (char* c, char* s, int start);
char* runassembler(FILE* f);

int ordertrans(char* order);

int lastLine;
FILE* inputFile;

typedef struct label {
  char *name;
  int value;
} LABEL;

LABEL lables[50];
int labelindex;

void handleorder(char* curline, int index);
void handleInstructions(char* curline, int index);
char* getcharstillchar(char* text, int start, char c);

#endif /* MMN14_H_ */
