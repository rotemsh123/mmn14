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

/*
 * this function check if this line is a macro line. if it start with the word "macro" it return 0. 1 if not
 */
int ismacroline(char* line) {
	if (strncmp(&line[ind], "macro", 5) == 0) {
		return 0;
	} else{

		return 1;
	}
}

/*
 * copy the next line to file. if this si the first line, ignore whatever already in the file
 */
void copyText(char* line, char* file, int firstline) {
	if (firstline == 0){
		strcat(file, line);
	}
	else{
		strcpy(file, line);
	}
}

/*
 * this function gets file and run the preasembler:
 * each macro definition is saved in macro table and then,
 * in any usage of this macro - it copies the text to replace the macro name
 */
char* preasembler(FILE* f) {
	char* curline;
	MACRO inmacro[10];
	int macronum;
	int i;
	char* file;
	int cleanfile;
	int macro = 0;
	int isMacro;
	int linenumber = 1;

	inmacro[0]=NULL;

	/*init global variables*/
	ind = 0;
	lastLine=0;
	inputFile = f;
	m[0]=NULL;

	file = (char*) malloc(10000);
	cleanfile = 1;
	curline = readline(f, linenumber++);
	macronum = 0;


	while (1) {
		ind = ignorewhitechar(curline,ind);
		isMacro = ismacroline(curline);

		/* not a macro name line*/
		if (isMacro == 1) {
			for (i = 0; i < macronum; i++) {
				if (compareignore(&(curline[ind]), m[i]) ==0) {
					copyText(inmacro[i], file, cleanfile);
					cleanfile = 0;
					macro = 1;
				}
			}
			if (macro == 0) {
				copyText(curline, file, cleanfile);
				cleanfile = 0;
			}
			macro = 0;
		}
		else {
			macroname(curline,macronum);
			inmacro[macronum] = (char*) malloc(200);
			curline = readline(f, linenumber++);
			if (curline[0] == EOF) {
				break;
			}
			ind=ignorewhitechar(curline,0);
			while (strncmp(&(curline[ind]), "endmacro", 8) != 0) {
				copyText(curline, inmacro[macronum], cleanfile);
				cleanfile = 0;
				curline = readline(f, linenumber++);
				ind = ignorewhitechar(curline,ind);
			}
			macronum++;
		}
		if (lastLine==1){
			break;
		}
		curline = readline(f, linenumber++);

		ind=0;
	}

	/*Release all memory*/
	free (curline);
	for (i = 0; i < macronum; i++) {
		free(m[i]);
		free (inmacro[i]);
	}

	return file;
}
