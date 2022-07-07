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
 * in the second round - skip the labels
 */
int skiplabel(int linenumber, char* curline, int index){

	int indexofdots;
	index = ignorewhitechar(curline, index);
	indexofdots = indexof(":", curline, index);
	if (indexofdots==-1){
		return index;
	}
	return indexofdots+1;
}


/*
 * check if there is label in this line, if there is,
 * takes the label and put it into the
 * label table with the current address
 */
int handlelabel(int linenumber, char* curline, int index){

	int indexofdots;
	char* label;
	int i;
	int instructiontype;
	index = ignorewhitechar(curline, index);
	indexofdots = indexof(":", curline, index);
	if (indexofdots==-1){
		return index;
	}
	/* if there is ":" in the line, we take the lable and handle it*/
	label = getcharstillchar(curline, index, ':');
	/* check if this label already exists*/

	for (i=0; i< labelindex; i++){
		if (strcmp(symboltable[i].name, label) == 0){
			printf ("ERROR in line %d. label: %s already exist\n", linenumber, label);
		}
	}

	/*Ignore label in .entry or .extern lines*/
	instructiontype = intructionlinetype(curline, indexofdots+1);
	if (instructiontype == 3){
		printf ("WARNING in line %d. label: %s is ignored because it is in .entry line\n", linenumber, label);
		return indexofdots+1;
	}
	if (instructiontype == 4){
		printf ("WARNING in line %d. label: %s is ignored because it is in .extern line\n", linenumber, label);
		return indexofdots+1;
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

void runassembler(char* filename){
	char* curline;
	FILE* amfile;
	int linenumber = 1;
	IC=100;
	DC=0;
	lastLine = 0;
	labelindex = 0;
	entryindex = 0;
	externindex =0;

	amfile = fopen (filename, "r");

	curline = readline(amfile);

	/*
	 * round 1: build the symbol table
	 */
	do{
		if (isEmptyLineOrComment(curline)!=0){

			int index = 0;
			index = handlelabel(linenumber, curline, index);
			if(isInstruction(curline, index)==0){
				symboltable[labelindex].value = DC;
				handleInstructionsLabel(linenumber, curline, index);
			}
			else {
				symboltable[labelindex].value = IC;
				handleorderLabel(linenumber, curline, index);
			}
		}
		curline = readline(amfile);
		linenumber++;
	}
	while (lastLine!=1);
	fclose(amfile);

	updatelabels();
	printlabels();

	/*
	 * round 2: build the rest
	 */
	printf ("\n\n*** Starting round 2 ***\n");
	IC = 100;
	DC = 0;
	amfile = fopen (filename, "r");
	linenumber = 1;
	lastLine = 0;
	initwords();
	curline = readline(amfile);

	do{
		if (isEmptyLineOrComment(curline)!=0){

			int index = 0;
			index = skiplabel(linenumber, curline, index);
			if(isInstruction(curline, index)==0){
				handleInstructions(linenumber, curline, index);
			}
			else {
				handleorder(linenumber, curline, index);
			}
		}
		curline = readline(amfile);
		linenumber++;
	}
	while (lastLine!=1);
	fclose(amfile);

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
		char* string = WORDtostringwithminus(w);
		int wint = WORDtoInt(w);
		char* i32 = trans32(wint);
		printf("Address: %d (%s), value %s (int %d,  32: %s)\n", i, trans32(i), string, wint, i32);
		fprintf(objectfile, "%s\t%s\n", trans32(i), i32);
	}
	printf("Starting Data\n");
	for (i=0; i<DC; i++){
		WORD w = datamemory[i];
		char* string = WORDtostringwithminus(w);
		int wint = WORDtoInt(w);
		char* i32 = trans32(wint);
		printf("Address: %d (%s), value %s (int %d,  32: %s)\n", i+IC, trans32(i+IC), string, wint, i32);
		fprintf(objectfile, "%s\t%s\n", trans32(i+IC), i32);
	}
	fclose(objectfile);

}

void saveentryfile(char* filename){
	char* entryfilename;
	FILE* entryfile;
	int i;
	if (entryindex==0){
		/*no entry in the file so file will not be created*/
		return;
	}
	entryfilename=(char*)malloc(80);
	strcpy(entryfilename,filename);
	strcat(entryfilename ,".ent");
	entryfile = fopen (entryfilename, "w");

	for (i=0; i<entryindex; i++){
		fprintf(entryfile, "%s\t%s\n", entry[i], trans32(getlabeladdress(entry[i])));
	}
	fclose(entryfile);
}

