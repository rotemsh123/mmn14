/*
 * preasembler.c
 *
 *  Created on: Jun 1, 2022
 *      Author: user
 */
#include "mmn14.h"
typedef char* MACRO;
int ind = 0;
int lastLine=0;
MACRO m[10];
FILE* inputFile;

void ignorewhitechar(char* line) {

	while(strncmp(&(line[ind]), " ",1) == 0 || strncmp(&(line[ind]), "\t",1) == 0) {
		ind++;
	}
}

void macroname(char* line,int i) {
	int charIndex = 0;
	m[i]=(char*)malloc(80);
	ind=0;
	ignorewhitechar(line);
	ind += 5;
	ignorewhitechar(line);
	while (strcmp(&(line[ind]), " ") != 0 && strcmp(&(line[ind]), "\t") != 0
			&& line[ind] != EOF && strcmp(&(line[ind]), "\n") != 0) {
		strncpy(&(m[i][charIndex]), &(line[ind]), 1);
		ind++;
		charIndex++;
	}
}

char* readline() {
	char* line;
	int i=0;
	line=(char*)malloc(80);

	while (1){
		if (fscanf(inputFile, "%c",&(line[i]))==1){
			if (strncmp(&(line[i]),"\n", 1)==0){
				/*printf("\n Line: %s \n ", line);*/
				return line;
			}
			i++;
		}
		else{
			/*printf ("LAST LINE");*/
			lastLine = 1;
			/*printf("\n Line: %s \n ", line);*/
			return line;
		}
	}

}

int ismacroline(char* line) {
	if (strncmp(&line[ind], "macro", 5) == 0) {
		return 0;
	} else{

		return 1;
	}
}

void copyText(char* line, char* file) {
	strcat(file, line);
}

char* preasembler(FILE* f) {
	char* curline;
	MACRO inmacro[10];
	int macronum;
	int i;
	char* file;
	int macro = 0;
	int isMacro;

	inmacro[0]=NULL;
	m[0]=NULL;
	file = (char*) malloc(10000);
	curline = readline();
	macronum = 0;
	inputFile = f;

	do {
		ignorewhitechar(curline);
		isMacro = ismacroline(curline);

		/* not a macro name line*/
		if (isMacro == 1) {
			for (i = 0; i < macronum; i++) {
				if (strncmp(&(curline[ind]), m[i],strlen(m[i])-1) ==0) {
					copyText(inmacro[i], file);
					macro = 1;
				}
			}
			if (macro == 0) {
				copyText(curline, file);
			}
			macro = 0;
		}
		else {
			macroname(curline,macronum);
			inmacro[macronum] = (char*) malloc(200);
			curline = readline();
			if (curline[0] == EOF) {
				break;
			}
			ind=0;
			ignorewhitechar(curline);
			while (strncmp(&(curline[ind]), "endmacro", 8) != 0) {
				copyText(curline, inmacro[macronum]);
				curline = readline();
				ignorewhitechar(curline);
			}
			macronum++;
		}

		curline = readline();
		ind=0;
	}
	while (lastLine!=1);

	/*printf("\nAfter last line:\n%s\n", file);
	printf("------------------------------------------\n");*/

	return file;
}
