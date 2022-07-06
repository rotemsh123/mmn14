/*
 * instructions.c
 *
 *  Created on: Jun 28, 2022
 *      Author: user
 */

#include "mmn14.h"

int stringtoint(char *string, int linenumber) {
	int result;
	int puiss;
	int index=0, length;
	index=ignorewhitechar(string,index);

	length = strlen(string);
	result = 0;
	puiss = 1;
	while (('-' == string[index]) || (string[index] == '+')) {
		if (string[index] == '-')
			puiss = puiss * -1;
		index++;
	}
	while (index<length) {
		if ((string[index] >= '0') && (string[index] <= '9')){
			result = (result * 10) + (string[index] - '0');
			index++;
		}
		else{
			printf("ERROR in line %d: Expecting number and got char: %c\n",linenumber, string[index]);
			break;
		}
	}

	return (result * puiss);
}

/**
 * convert int into word
 */
void inttoword(char* string, int linenumber, WORD* w){
	int d;
	int negative=0, j;
	negative = 0;
	d = stringtoint(string, linenumber);

	printf("char is: %s and int is: %d\n", string, d);
	j = 0;

	if (d<0){
		d*=-1;
		negative = 1;
	}
	while (d != 0) {
		w[0].value[j] = d % 2;
		d /= 2;
		j++;
	}
	/*if number is negative, I switch each 0 to 1 and than add 1*/
	if (negative == 1){
		for (j=0; j<10; j++){
			if(w[0].value[j] == 0){
				w[0].value[j] = 1;
			}
			else {
				w[0].value[j] = 0;
			}
		}
		/*now should add 1*/
		for (j=0; j<10; j++){
			if(w[0].value[j] == 0){
				w[0].value[j] = 1;
				break;
			}
			else{
				w[0].value[j] = 0;
			}
		}
	}

}
void handledata(int linenumber, char* curline, int index) {
	char* string;
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

		inttoword(string, linenumber,&datamemory[DC]);
		printf("Word is: %s\n", WORDtostring(datamemory[DC]));

		DC++;
	}
}

void putstringindata(char* string){
	int i, j;
	for (i = 0; i < strlen(string); i++) {

		char c = string[i];
		int d = (int) c;

		printf("char %c is %d\n", c, d);
		j = 0;

		while (d > 0) {
			datamemory[DC].value[j] = d % 2;
			d /= 2;
			j++;
		}
		printf("Word is: %s\n", WORDtostring(datamemory[DC]));

		DC++;
	}
	/*
	 * next line is zero at the end of the string
	 */
	DC++;

}
void handlestring(int linenumber, char* curline, int index) {
	char* string;
	index = index + 7;
	index = ignorewhitechar(curline, index);
	if (indexof(",", curline, index) != -1) {
		printf("ERROR in line %d: Illegal char in string (','): %s",linenumber, &(curline[index]));
	}
	index = indexof("\"", curline, index) + 1;
	string = getcharstillchar(curline, index, '\"');
	putstringindata(string);

	printf("String: %s\n", string);
}
void handlestruct(int linenumber, char* curline, int index) {
	char* arg1;
	char* arg2;

	if (indexof(",", curline, index) ==-1){
		printf("ERROR in line %d: missing operands for struct: %s \n", linenumber, &(curline[index]));
		return;
	}
	index=indexof(" ", curline, index)+1;
	ignorewhitechar(curline, index);
	arg1 = getcharstillchar(curline, index, ',');
	index = indexof(",", curline, index) +1;
	index = indexof("\"", curline, index) + 1;
	arg2 = getcharstillchar(curline, index, '\"');

	/* First word is int to word*/
	inttoword(arg1, linenumber, &datamemory[DC]);
	printf("Word is: %s\n", WORDtostring(datamemory[DC]));
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
