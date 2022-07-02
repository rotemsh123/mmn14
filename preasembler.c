/*
 * preasembler.c
 *
 *  Created on: Jun 1, 2022
 *      Author: user
 */
#include "mmn14.h"
typedef char* MACRO;
int ind;
MACRO m[10];


void macroname(char* line,int i) {
	int charIndex = 0;
	m[i]=(char*)malloc(80);
	ind=ignorewhitechar(line,0);
	ind += 5;
	ind = ignorewhitechar(line,ind);
	while (strcmp(&(line[ind]), " ") != 0 && strcmp(&(line[ind]), "\t") != 0
			&& line[ind] != EOF && strcmp(&(line[ind]), "\n") != 0) {
		strncpy(&(m[i][charIndex]), &(line[ind]), 1);
		ind++;
		charIndex++;
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

	/*init global variables*/
	ind = 0;
	lastLine=0;
	inputFile = f;
	m[0]=NULL;

	file = (char*) malloc(10000);
	curline = readline(f);
	macronum = 0;


	do {
		ind = ignorewhitechar(curline,ind);
		isMacro = ismacroline(curline);

		/* not a macro name line*/
		if (isMacro == 1) {
			for (i = 0; i < macronum; i++) {
				if (compareignore(&(curline[ind]), m[i]) ==0) {
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
			curline = readline(f);
			if (curline[0] == EOF) {
				break;
			}
			ind=ignorewhitechar(curline,0);
			while (strncmp(&(curline[ind]), "endmacro", 8) != 0) {
				copyText(curline, inmacro[macronum]);
				curline = readline();
				ind = ignorewhitechar(curline,ind);
			}
			macronum++;
		}
		curline = readline(f);

		ind=0;
	}
	while (lastLine!=1);

	/*Release all memory*/
	free (curline);
	for (i = 0; i < macronum; i++) {
		free(m[i]);
		free (inmacro[i]);
	}

	return file;
}
