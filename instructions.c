/*
 * instructions.c
 *
 *  Created on: Jun 28, 2022
 *      Author: user
 */

#include "mmn14.h"

int stringtoint(char *str) {
	int result;
	int puiss;

	result = 0;
	puiss = 1;
	while (('-' == (*str)) || ((*str) == '+')) {
		if (*str == '-')
			puiss = puiss * -1;
		str++;
	}
	while ((*str >= '0') && (*str <= '9')) {
		result = (result * 10) + ((*str) - '0');
		str++;
	}
	return (result * puiss);
}

/**
 * convert int into word
 */
WORD inttoword(char* string){
	int negative=0, j;
	negative = 0;
	WORD w;
	int d = stringtoint(string);
	initword(&w);
	printf("char is: %s and int is: %d\n", string, d);
	j = 0;

	if (d<0){
		d*=-1;
		negative = 1;
	}
	while (d != 0) {
		w.value[j] = d % 2;
		d /= 2;
		j++;
	}
	/*if number is negative, I switch each 0 to 1 and than add 1*/
	if (negative == 1){
		for (j=0; j<10; j++){
			if(w.value[j] == 0){
				w.value[j] = 1;
			}
			else {
				w.value[j] = 0;
			}
		}
		/*now should add 1*/
		for (j=0; j<10; j++){
			if(w.value[j] == 0){
				w.value[j] = 1;
				break;
			}
			else{
				w.value[j] = 0;
			}
		}
	}
	return w;
}
void handledata(int linenumber, char* curline, int index) {
	char* string;
	WORD w;
	int lastarg = 0;
	index = index + 5;
	index = ignorewhitechar(curline, index);

	while (lastarg == 0) {

		if (indexof(",", curline, index) != -1) {
			string = getcharstillchar(curline, index, ',');
			index = indexof(",", curline, index) + 1;
		} else {
			string = getcharstillchar(curline, index, '\n');
			lastarg = 1;
		}

		w = inttoword(string);
		printf("Word is: %s\n", WORDtostring(w));
		datamemory[DC] = &w;
		DC++;
	}
}

void putstringindata(char* string){
	int i, j;
	for (i = 0; i < strlen(string); i++) {
		WORD w;
		char c = string[i];
		int d = (int) c;
		initword(&w);
		printf("char %c is %d\n", c, d);
		j = 0;
		while (d > 0) {
			w.value[j] = d % 2;
			d /= 2;
			j++;
		}
		printf("Word is: %s\n", WORDtostring(w));
		datamemory[DC] = &w;
		DC++;
	}
	/*
	 * Add zero line at the end of the string
	 */
	WORD w;
	initword(&w);
	datamemory[DC] = &w;
	DC++;

}
void handlestring(int linenumber, char* curline, int index) {
	char* string;
	index = index + 7;
	index = ignorewhitechar(curline, index);
	if (indexof(",", curline, index) != -1) {
		printf("Error in line %d: Illegal char in string (','): %s \n",
				linenumber, &(curline[index]));
	}
	index = indexof("\"", curline, index) + 1;
	string = getcharstillchar(curline, index, '\"');
	putstringindata(string);

	printf("String: %s\n", string);
}
void handlestruct(int linenumber, char* curline, int index) {
	char* arg1;
	char* arg2;
	WORD arg1word;
	if (indexof(",", curline, index) ==-1){
		printf("Error in line %d: missing operands for struct: %s \n", linenumber, &(curline[index]));
		return;
	}
	index=indexof(" ", curline, index)+1;
	ignorewhitechar(curline, index);
	arg1 = getcharstillchar(curline, index, ',');
	index = indexof(",", curline, index) +1;
	index = indexof("\"", curline, index) + 1;
	arg2 = getcharstillchar(curline, index, '\"');

	/* First word is int to word*/
	arg1word = inttoword(arg1);
	printf("Word is: %s\n", WORDtostring(arg1word));
	datamemory[DC] = &arg1word;
	DC++;

	/* Next words are the string*/
	putstringindata(arg2);

}
void handleentry(int linenumber, char* curline, int index) {
	index = index + 6;
	printf("Entry: %s", &(curline[index]));
}
void handleextern(int linenumber, char* curline, int index) {
	index = index + 7;
	printf("Extern: %s", &(curline[index]));
}

void handleInstructions(int linenumber, char* curline, int index) {
	printf("instruction line: %s", &(curline[index]));
	if (islabelline == 1) {
		symboltable[labelindex].value = DC;
		symboltable[labelindex].DI = 1;
		labelindex++;
		islabelline = 0;
	}

	index = ignorewhitechar(curline, index);
	if (strncmp(&(curline[index]), ".data ", 6) == 0) {
		handledata(linenumber, curline, index);
	} else if (strncmp(&(curline[index]), ".string ", 8) == 0) {
		handlestring(linenumber, curline, index);
	} else if (strncmp(&(curline[index]), ".struct ", 8) == 0) {
		handlestruct(linenumber, curline, index);
	} else if (strncmp(&(curline[index]), ".entry ", 7) == 0) {
		handleentry(linenumber, curline, index);
	} else if (strncmp(&(curline[index]), ".extern ", 8) == 0) {
		handleextern(linenumber, curline, index);
	} else {
		printf("ERROR in line %d: Illegal instruction: %s", linenumber,
				&(curline[index]));
	}

}
