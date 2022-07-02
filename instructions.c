/*
 * instructions.c
 *
 *  Created on: Jun 28, 2022
 *      Author: user
 */

#include "mmn14.h"

void handledata(char* curline, int index){
	index = index + 5;
	printf("Data: %s", &(curline[index]));
}
void handlestring(char* curline, int index){
	index = index + 7;
	printf("String: %s", &(curline[index]));
}
void handlestruct(char* curline, int index){
	index = index + 7;
	printf("Struct: %s", &(curline[index]));
}
void handleentry(char* curline, int index){
	index = index + 6;
	printf("Entry: %s", &(curline[index]));
}
void handleextern(char* curline, int index){
	index = index + 7;
	printf("Extern: %s", &(curline[index]));
}


void handleInstructions(char* curline, int index){
	printf("instruction line: %s", &(curline[index]));
	index = ignorewhitechar(curline, index);
	if (strncmp(&(curline[index]), ".data", 5) ==0){
		handledata(curline, index);
		return;
	}
	if (strncmp(&(curline[index]), ".string", 7) ==0){
		handlestring(curline, index);
		return;
	}
	if (strncmp(&(curline[index]), ".struct", 7) ==0){
		handlestruct(curline, index);
		return;
	}
	if (strncmp(&(curline[index]), ".entry", 6) ==0){
		handleentry(curline, index);
		return;
	}
	if (strncmp(&(curline[index]), ".extern", 7) ==0){
		handleextern(curline, index);
		return;
	}
	printf("Illegal instruction: %s", &(curline[index]));

}
