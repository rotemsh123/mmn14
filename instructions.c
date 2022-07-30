/*
 * instructions.c
 *
 *  Created on: Jun 28, 2022
 *      Author: user
 */

#include "mmn14.h"



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

		inttoword(string, linenumber,&datamemory[DC], 1);
		if (VERBOSS > 2){
			printf("Word is: %s\n", WORDtostringwithminus(datamemory[DC]));
		}

		DC++;
	}
}

/*
 * putstringindata takes string, split it into chars and put each char as a WORD in the data memory
 */
void putstringindata(char* string){
	int i, j;
	for (i = 0; i < strlen(string); i++) {

		char c = string[i];
		int d = (int) c;
		if (VERBOSS > 2){
			printf("char %c is %d\n", c, d);
		}
		j = 0;

		while (d > 0) {
			datamemory[DC].value[j] = d % 2;
			d /= 2;
			j++;
		}
		if (VERBOSS > 2){
			printf("Word is: %s\n", WORDtostring(datamemory[DC]));
		}
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
		if (VERBOSS > 0){
			printf("ERROR in line %d: Illegal char in string (','): %s",linenumber, &(curline[index]));
		}
	}
	index = indexof("\"", curline, index) + 1;
	string = getcharstillchar(curline, index, '\"');
	putstringindata(string);
	if (VERBOSS > 2){
		printf("String: %s\n", string);
	}
}



/*
 * handle struct. where first argument is int and second is string
 */
void handlestruct(int linenumber, char* curline, int index) {
	char* arg1;
	char* arg2;

	if (indexof(",", curline, index) ==-1){
		if (VERBOSS > 0){
			printf("ERROR in line %d: missing operands for struct: %s \n", linenumber, &(curline[index]));
		}
		return;
	}
	index=indexof(" ", curline, index)+1;
	ignorewhitechar(curline, index);
	arg1 = getcharstillchar(curline, index, ',');
	index = indexof(",", curline, index) +1;
	index = indexof("\"", curline, index) + 1;
	arg2 = getcharstillchar(curline, index, '\"');

	/* First word is int to word*/
	inttoword(arg1, linenumber, &datamemory[DC], 1);
	if (VERBOSS > 2){
		printf("Word is: %s\n", WORDtostringwithminus(datamemory[DC]));
	}
	DC++;

	/* Next words are the string*/
	putstringindata(arg2);

}

/*
 * handle line when it is entry
 * this function takes the argument after the .entry and puts it in the entry table for future usage
 */
void handleentry(int linenumber, char* curline, int index) {
	char* entrychar;
	index = index + 6;
	index = ignorewhitechar(curline, index);
	entrychar = getcharstillchar(curline, index, '\n');
	if (VERBOSS > 2){
		printf("Entry: %s\n", entrychar);
	}
	entry[entryindex] = entrychar;
	entryindex++;
}

/*
 * handle line when it is external
 * this function takes the argument after the .external and puts it in the external structure for future usage
 * the reason to use struct is because for each usage of the external lable, we should keep the adress where it is used
 */
void handleextern(int linenumber, char* curline, int index) {
	char* externalchar;
	int i;
	index = index + 7;
	index = ignorewhitechar(curline, index);
	externalchar = getcharstillchar(curline, index, '\n');
	if (VERBOSS > 2){
		printf("Extern: %s\n", externalchar);
	}
	for (i=0; i<externindex; i++){
		if (strcmp (external[i].name, externalchar) ==0){
			if (VERBOSS > 0){
				printf("ERROR in line %d: external already exist: %s \n", linenumber, externalchar);
			}
			return;
		}
	}
	external[externindex].name = (char*)malloc (30);
	strcpy(external[externindex].name, externalchar);
	externindex++;
}

/*
 * 0 for .data
 * 1 for .string
 * 2 for .struct
 * 3 for .entry
 * 4 for .extern
 * -1 for error
 */
int intructionlinetype(char* curline, int index){
	index = ignorewhitechar(curline, index);
	if (strncmp(&(curline[index]), ".data ", 6) == 0) {
		return 0;
	}
	if (strncmp(&(curline[index]), ".string ", 8) == 0) {
		return 1;
	}
	if (strncmp(&(curline[index]), ".struct ", 8) == 0) {
		return 2;
	}
	if (strncmp(&(curline[index]), ".entry ", 7) == 0) {
		return 3;
	}
	if (strncmp(&(curline[index]), ".extern ", 8) == 0) {
		return 4;
	}
	return -1;

}

/*
 * This function is handeling line when it contains instruction
 */
void handleInstructions(int linenumber, char* curline, int index) {
	int instructiontype;
	index = ignorewhitechar(curline, index);
	instructiontype = intructionlinetype(curline, index);
	if (VERBOSS > 2){
		printf("instruction line: %s\n", &(curline[index]));
	}

	if (instructiontype == 0) {
		handledata(linenumber, curline, index);
	}
	else if (instructiontype == 1) {
		handlestring(linenumber, curline, index);
	}
	else if (instructiontype == 2) {
		handlestruct(linenumber, curline, index);
	}
	else if (instructiontype == 3) {
		handleentry(linenumber, curline, index);
	}
	else if (instructiontype == 4) {
		handleextern(linenumber, curline, index);
	}
	else {
		if (VERBOSS > 0){
			printf("ERROR in line %d: Illegal instruction: %s", linenumber,
				&(curline[index]));
		}
	}

}


/*
 *
 * all functions for round 1
 */



/*
 * This function is used for round 1 and counting the number of words needed for data.
 * for each argument in the data line it adds 1 WORD
 */
void handledataLabel(int linenumber, char* curline, int index) {
	int lastarg = 0;
	index = index + 5;
	index = ignorewhitechar(curline, index);

	while (lastarg == 0) {

		if (indexof(",", curline, index) != -1) {
			index = indexof(",", curline, index) + 1;
		} else {
			lastarg = 1;
		}
		DC++;
	}
}

/*
 * This function is used for round 1 and counting the number of words needed for string.
 * for each char in the string it adds 1 WORD
 */
void handlestringLabel(int linenumber, char* curline, int index) {
	char* string;
	int i;
	index = index + 7;
	index = ignorewhitechar(curline, index);
	index = indexof("\"", curline, index) + 1;
	string = getcharstillchar(curline, index, '\"');
	for (i = 0; i <= strlen(string); i++) {
		DC++;
	}
}

/*
 * This function is used for round 1 and counting the number of words needed for struct.
 * for the first int it allocate 1 WORD
 * and for each char in the string it adds 1 WORD
 */

void handlestructLabel(int linenumber, char* curline, int index) {
	DC++;
	handlestringLabel (linenumber, curline, index);
}

/*
 * This function is used for round 1 and counting the number of words needed for the instruction line.
 */

void handleInstructionsLabel(int linenumber, char* curline, int index) {
	int instructiontype;
	index = ignorewhitechar(curline, index);
	instructiontype = intructionlinetype(curline, index);

	if (islabelline == 1) {
		symboltable[labelindex].value = DC;
		symboltable[labelindex].DI = 1;
		labelindex++;
		islabelline = 0;
	}
	if (instructiontype == 0) {
		handledataLabel(linenumber, curline, index);
	}
	else if (instructiontype == 1) {
		handlestringLabel(linenumber, curline, index);
	}
	else if (instructiontype == 2) {
		handlestructLabel(linenumber, curline, index);
	}

}
