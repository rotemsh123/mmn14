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
typedef struct {
	int value[10];
} REGISTER;

typedef struct{
	int value[10];
} WORD;

WORD* memory[256];
int IC;
int DC;
char* readline();
char* preasembler(FILE* f);
int ignorewhitechar(char* line,int ind);
int compareignore(char* s1, char* s2);
int indexof (char* c, char* s, int start);
char* runassembler(FILE* f);

int ordertrans(char* order);

int lastLine;
FILE* inputFile;

typedef struct {
  char *name;
  int value;
  int type; /*0 = relocatable 1= external*/
} LABEL;

LABEL symboltable[50];
int labelindex;

void handleorder(int linenumber, char* curline, int index);
void handleInstructions(int linenumber, char* curline, int index);
char* getcharstillchar(char* text, int start, char c);
int getaddresscode(char* arg);
char* WORDtostring(WORD word);
char* WORDtostringwithminus(WORD word);

#endif /* MMN14_H_ */
