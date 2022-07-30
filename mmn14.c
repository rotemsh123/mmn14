#include "mmn14.h"
REGISTER r0, r1, r2, r3, r4, r5, r6, r7, PSW;
FILE* inputFile;
FILE* aftermacrofile;

/*
 * This function handle 1 file of .as.
 * run the preassembler for the macros and then run the 2 rounds of the assembler to create all the needed output files
 */
void handlefile(char* filename){
	char* text;
	char* inputfilename;
	char* fileaftermacro;

	inputfilename=(char*)malloc(80);
	strcpy(inputfilename,filename);
	strcat(inputfilename ,".as");

	inputFile = fopen(inputfilename, "r");

	if (inputFile == NULL || inputFile == NULL)
	{
		if (VERBOSS > 0){
			printf("ERROR. Could not open file\n");
		}
		exit(-1);
	}
	text = preasembler(inputFile);
	puts(text);

	fclose(inputFile);

	fileaftermacro=(char*)malloc(800);
	strcpy(fileaftermacro,filename);
	strcat(fileaftermacro, ".am");
	aftermacrofile = fopen (fileaftermacro, "w");
	fprintf(aftermacrofile, "%s", text);
	fclose(aftermacrofile);

	runassembler(fileaftermacro);

	free (inputfilename);
	/*free (text);*/
	free (fileaftermacro);

	if (ERROR == 0){
		saveobjectfile(filename);
		saveentryfile(filename);
		saveexternalfile(filename);
	}
}

/*
 * main function of the program
 * it gets list of file names and handle each one of them
 */
int main(int argc, char **argv) {
	int i;
	VERBOSS = 2;
	ERROR=0;
	for (i=1; i<argc; i++){
		char* filename = argv[i];
		if (VERBOSS > 2){
			printf ("Handeling file: %s\n", filename);
		}
		handlefile(filename);
	}

	return 0;
}
