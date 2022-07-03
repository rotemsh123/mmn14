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
int handlelabel(char* line, int index){

	int indexofdots;
	char* label;
	index = ignorewhitechar(line, index);
	indexofdots = indexof(":", line, index);
	if (indexofdots==-1){
		return index;
	}
	// if there is ":" in the line, we take the lable and handle it
	labelindex++;
	label = getcharstillchar(line, index, ':');
	lables[labelindex].name = (char*)malloc(30);
	strcpy(lables[labelindex].name, label);
	lables[labelindex].value = PC;

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
	lastLine = 0;
	labelindex = 0;
	text = (char*)malloc (1000);
	curline = readline(f);
	do{
		if (isEmptyLineOrComment(curline)!=0){

			int index = 0;
			index = handlelabel(curline, index);
			if(isInstruction(curline, index)==0){
				handleInstructions(curline, index);
			}
			else {
				handleorder(curline, index);
			}
		}
		curline = readline(f);
	}
	while (lastLine!=1);

	return text;
}
