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
	index = index + 7;
	printf("String: %s", &(curline[index]));
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
	index = ignorewhitechar(curline, index);
	if (strncmp(&(curline[index]), ".data ", 6) ==0){
		handledata(linenumber, curline, index);
		return;
	}
	if (strncmp(&(curline[index]), ".string ", 8) ==0){
		handlestring(linenumber, curline, index);
		return;
	}
	if (strncmp(&(curline[index]), ".struct ", 8) ==0){
		handlestruct(linenumber, curline, index);
		return;
	}
	if (strncmp(&(curline[index]), ".entry ", 7) ==0){
		handleentry(linenumber, curline, index);
		return;
	}
	if (strncmp(&(curline[index]), ".extern ", 8) ==0){
		handleextern(linenumber, curline, index);
		return;
	}
	printf("ERROR in line %d: Illegal instruction: %s", linenumber, &(curline[index]));

}
