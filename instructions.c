/*
 * instructions.c
 *
 *  Created on: Jun 28, 2022
 *      Author: user
 */

#include "mmn14.h"

void handledata(int linenumber, char* curline, int index){
	index = index + 5;
	printf("Data: %s", &(curline[index]));
}
void handlestring(int linenumber, char* curline, int index){
	char* string;
	int i,j;
	index = index + 7;
	index = ignorewhitechar(curline, index);
	if (indexof(",", curline, index) != -1){
		printf("Error in line %d: Illegal char in string (','): %s \n", linenumber, &(curline[index]));
	}
	index = indexof("\"", curline, index)+1;
	string = getcharstillchar(curline, index, '\"');
	for (i=0;i<strlen(string); i++){
		WORD w;
		char c = string[i];
		int d = (int)c;
		initword(&w);
		printf ("char %c is %d\n", c, d);
		j=0;
		while (d>0){
			w.value[j]=d % 2;
			d /=2;
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
	printf("String: %s\n", string);
}
void handlestruct(int linenumber, char* curline, int index){
	index = index + 7;
	printf("Struct: %s", &(curline[index]));
}
void handleentry(int linenumber, char* curline, int index){
	index = index + 6;
	printf("Entry: %s", &(curline[index]));
}
void handleextern(int linenumber, char* curline, int index){
	index = index + 7;
	printf("Extern: %s", &(curline[index]));
}


void handleInstructions(int linenumber, char* curline, int index){
	printf("instruction line: %s", &(curline[index]));
	if (islabelline == 1){
		symboltable[labelindex].value = DC;
		symboltable[labelindex].DI = 1;
		labelindex++;
		islabelline=0;
	}

	index = ignorewhitechar(curline, index);
	if (strncmp(&(curline[index]), ".data ", 6) ==0){
		handledata(linenumber, curline, index);
	}
	else if (strncmp(&(curline[index]), ".string ", 8) ==0){
		handlestring(linenumber, curline, index);
	}
	else if (strncmp(&(curline[index]), ".struct ", 8) ==0){
		handlestruct(linenumber, curline, index);
	}
	else if (strncmp(&(curline[index]), ".entry ", 7) ==0){
		handleentry(linenumber, curline, index);
	}
	else if (strncmp(&(curline[index]), ".extern ", 8) ==0){
		handleextern(linenumber, curline, index);
	}
	else{
		printf("ERROR in line %d: Illegal instruction: %s", linenumber, &(curline[index]));
	}


}
