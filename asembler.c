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
			if (VERBOSS > 0){
				printf ("ERROR in line %d: Label: %s already exists\n", linenumber, label);
				ERROR = 1;
			}
		}
	}

	/*Ignore label in .entry or .extern lines*/
	instructiontype = intructionlinetype(curline, indexofdots+1);
	if (instructiontype == 3){
		if (VERBOSS > 1){
			printf ("WARNING in line %d: Label: %s is ignored because it is in .entry line\n", linenumber, label);
		}
		return indexofdots+1;
	}
	if (instructiontype == 4){
		if (VERBOSS > 1){
			printf ("WARNING in line %d: Label: %s is ignored because it is in .extern line\n", linenumber, label);
		}
		return indexofdots+1;
	}
	islabelline =1;
	symboltable[labelindex].name = (char*)malloc(30);
	strcpy(symboltable[labelindex].name, label);

	if (VERBOSS > 2){
		printf ("label: %s\n", label);
	}
	return indexofdots+1;
}

/*
 * return 0 if the instruction starts with '.' - meaning it is instruction and not order
 */
int isInstruction(char* curline, int index){
	index = ignorewhitechar(curline, index);
	if (strncmp(".", &(curline[index]),1) ==0){
		return 0;
	}

	return 1;
}

/*
 * this function free all used memeory
 */
void cleanallmemory(){
	int i;
	for (i=0; i< labelindex; i++){
		free (symboltable[i].name);
	}
	for (i=0; i< externindex; i++){
		free (external[i].name);
	}

}

/*
 * this function run the assembler program on specific file
 */
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
	while(1){
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
		if (lastLine==1){
			break;
		}
		curline = readline(amfile);
		linenumber++;
	}

	fclose(amfile);

	updatelabels();
	printlabels();

	/*
	 * round 2: build the rest
	 */
	if (VERBOSS > 2){
		printf ("\n\n*** Starting round 2 ***\n");
	}
	IC = 100;
	DC = 0;
	amfile = fopen (filename, "r");
	linenumber = 1;
	lastLine = 0;
	initwords();
	curline = readline(amfile);

	while (1){
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
		if (lastLine==1){
			break;
		}
		curline = readline(amfile);
		linenumber++;
	}

	fclose(amfile);
	cleanallmemory();

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
		char* line32 = trans32(i);
		if (VERBOSS > 2){
			printf("Address: %d (%s), value %s (int %d,  32: %s)\n", i, line32, string, wint, i32);
		}
		fprintf(objectfile, "%s\t%s\n", line32, i32);
		free (i32);
		free (line32);
		free (string);
	}
	if (VERBOSS > 2){
		printf("Starting Data\n");
	}
	for (i=0; i<DC; i++){
		WORD w = datamemory[i];
		char* string = WORDtostringwithminus(w);
		int wint = WORDtoInt(w);
		char* i32 = trans32(wint);
		char* line32 = trans32(i+IC);
		if (VERBOSS > 2){
			printf("Address: %d (%s), value %s (int %d,  32: %s)\n", i+IC, line32, string, wint, i32);
		}
		fprintf(objectfile, "%s\t%s\n", line32, i32);
		free (i32);
		free (line32);
		free (string);

	}
	fclose(objectfile);
	free (objectfilename);

}

/*
 * this function create the .ent file and write all the entry lables addresses there.
 * if no entry exists - file will not be created.
 */
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
	free (entryfilename);
}

/*
 * this function create the .ext file and write all the external lables and the adresses where they are used.
 * if no external exists - file will not be created.
 */

void saveexternalfile(char* filename){
	char* externalfilename;
	FILE* externalfile;
	int i,j;
	if (externindex==0){
		/*no externals in the file so file will not be created*/
		return;
	}
	externalfilename=(char*)malloc(80);
	strcpy(externalfilename,filename);
	strcat(externalfilename ,".ext");
	externalfile = fopen (externalfilename, "w");

	for (i=0; i<entryindex; i++){
		for (j=0; j<external[i].numofaddresses; j++){
			fprintf(externalfile, "%s\t%s\n", external[i].name, trans32(external[i].addresses[j]));
		}
	}
	fclose(externalfile);
	free (externalfilename);
}

