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
 * After first execution, update the symbol table with the "real" adrresses
 * each symbol which point to the data add the IC value
 */
void updatelabels(){
	int i;
	for (i=0; i< labelindex; i++){
		if (symboltable[i].DI == 1){
			symboltable[i].value += IC;
		}
	}
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

	islabelline =1;
	symboltable[labelindex].name = (char*)malloc(30);
	strcpy(symboltable[labelindex].name, label);

	/*printf ("label: %s\n", label);*/
	return indexofdots+1;
}

int isInstruction(char* curline, int index){
	index = ignorewhitechar(curline, index);
	if (strncmp(".", &(curline[index]),1) ==0){
		return 0;
	}
	return 1;
}

void runassembler(FILE* f){
	char* curline;
	int linenumber = 0;
	IC=100;
	DC=0;
	lastLine = 0;
	labelindex = 0;
	initwords();

	curline = readline(f);
	linenumber++;
	/*
	 * round 1
	 */
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

	updatelabels();
	printlabels();

	/*
	 * round 2
	 */


}

/*
 * this function goes over all memory and same it to the file in base 32
 */
void saveobjectfile(char* filename){
	char* objectfilename;
	FILE* objectfile;
	int i;
	objectfilename=(char*)malloc(80);
	strcpy(objectfilename,filename);
	strcat(objectfilename ,".ob");
	objectfile = fopen (objectfilename, "w");

	for (i=100; i<IC; i++){
		WORD w = memory[i];
		char* string = WORDtostring(w);
		int wint = WORDtoInt(w);
		char* i32 = trans32(wint);
		printf("Address: %d (%s), value %s (int %d,  32: %s)\n", i, trans32(i), string, wint, i32);
		fprintf(objectfile, "%s\t%s\n", trans32(i), i32);
	}

	fclose(objectfile);

}
