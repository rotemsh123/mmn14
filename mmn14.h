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
	char* labelname; /*If the value is label name, only in second round I can find it, so keep here the label name for now*/
} WORD;

WORD memory[256];
WORD datamemory[256];
char* entry[100];
int entryindex;
typedef struct{
	char* name;
	int addresses [50];
	int numofaddresses;
}EXTERNAL;

EXTERNAL external[100];

int externindex;

int IC;
int DC;

int ERROR;
/*
 * verbos is log level. 0 is quite, 1 is error, 2 is warning, 3 is all
 */
int VERBOSS;
int islabelline;
char* readline(FILE* f, int linenumber);
char* preasembler(FILE* f);
int ignorewhitechar(char* line,int ind);
int compareignore(char* s1, char* s2);
int indexof (char* c, char* s, int start);
int hassomethingtillendofline(char* line, int start);
void runassembler(char* filename);
void saveobjectfile(char* filename);
void saveentryfile(char* filename);
void saveexternalfile(char* filename);
void cleanallmemory();

int ordertrans(char* order);

int lastLine;
FILE* inputFile;

typedef struct {
  char *name;
  int value;
  int type; /*0 = relocatable 1= external*/
  int DI; /* 0 is order, 1 is data*/
} LABEL;

LABEL symboltable[50];
int labelindex;

void handleorderLabel(int linenumber, char* curline, int index);
void handleInstructionsLabel(int linenumber, char* curline, int index);

void handleorder(int linenumber, char* curline, int index);
void handleInstructions(int linenumber, char* curline, int index);
char* getcharstillchar(char* text, int start, char c);
int getaddresscode(char* arg);
char* WORDtostring(WORD word);
char* WORDtostringwithminus(WORD word);
void printlabels();
void initwords();
int WORDtoInt(WORD w);
char* trans32(int number);
int intructionlinetype(char* curline, int index);
int getlabeladdress(char* labelname);
void inttoword(char* string, int linenumber, WORD* w, int dataordirect);



#endif /* MMN14_H_ */
