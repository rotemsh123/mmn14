#include "mmn14.h"


int isEmptyLineOrComment(char* line)
{
	int ind=0;
	ignorewhitechar(line,ind);
	if(line[ind]=='\n' || line[ind]==';'){
		return 0;
	}

	return 1;
}




/*
 * check if there is label in this line, if there is,
 * takes the label and put it into the
 * label table with the current address
 */
int handlelabel(int linenumber, char* line, int index){

	int indexofdots;
	char* label;
	int i;
	index = ignorewhitechar(line, index);
	indexofdots = indexof(":", line, index);
	if (indexofdots==-1){
		return index;
	}
	/* if there is ":" in the line, we take the lable and handle it*/
	label = getcharstillchar(line, index, ':');
	/* check if this label already exists*/

	for (i=0; i< labelindex; i++){
		if (strcmp(symboltable[i].name, label) == 0){
			printf ("ERROR in line %d. label: %s already exist\n", linenumber, label);
		}
	}

	symboltable[labelindex].name = (char*)malloc(30);
	strcpy(symboltable[labelindex].name, label);
	labelindex++;

	printf ("label: %s\n", label);
	return indexofdots+1;
}

int isInstruction(char* curline, int index){
	index = ignorewhitechar(curline, index);
	if (strncmp(".", &(curline[index]),1) ==0){
		return 0;
	}
	return 1;
}

char* runassembler(FILE* f){
	char* text;
	char* curline;
	int linenumber = 0;
	IC=100;
	DC=0;
	lastLine = 0;
	labelindex = 0;
	text = (char*)malloc (1000);
	curline = readline(f);
	linenumber++;
	do{
		if (isEmptyLineOrComment(curline)!=0){

			int index = 0;
			index = handlelabel(linenumber, curline, index);
			if(isInstruction(curline, index)==0){
				symboltable[labelindex].value = DC;
				handleInstructions(linenumber, curline, index);
			}
			else {
				symboltable[labelindex].value = IC;
				handleorder(linenumber, curline, index);
			}
		}
		curline = readline(f);
		linenumber++;
	}
	while (lastLine!=1);

	return text;
}
