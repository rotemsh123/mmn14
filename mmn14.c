#include "mmn14.h"
REGISTER r0, r1, r2, r3, r4, r5, r6, r7, PSW;
FILE* inputFile;
FILE* aftermacrofile;
FILE* amfile;

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
	  printf("Error! Could not open file\n");
	  exit(-1);
	}
	text = preasembler(inputFile);
	puts(text);

	fclose(inputFile);

	fileaftermacro=(char*)malloc(80);
	strcpy(fileaftermacro,filename);
	strcat(fileaftermacro, ".am");
	aftermacrofile = fopen (fileaftermacro, "w");
	fprintf(aftermacrofile, "%s", text);
	fclose(aftermacrofile);

	amfile = fopen (fileaftermacro, "r");
	runassembler(aftermacrofile);
	fclose(amfile);

	free (inputfilename);
	/*free (text);*/
	free (fileaftermacro);

	saveobjectfile(filename);
}

int main(int argc, char **argv) {
	int i;

	for (i=1; i<argc; i++){
		char* filename = argv[i];
		printf ("Handeling file: %s\n", filename);
		handlefile(filename);
	}

	return 0;
}
