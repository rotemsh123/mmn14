#include "mmn14.h"

int ignorewhitechar(char* line,int ind) {

	while(strncmp(&(line[ind]), " ",1) == 0 || strncmp(&(line[ind]), "\t",1) == 0) {
		ind++;
	}
	return ind;
}

char* readline(FILE* f) {
	char* line;
	int i=0;
	line=(char*)malloc(80);

	while (1){
		if (fscanf(f, "%c",&(line[i]))==1){
			if (strncmp(&(line[i]),"\n", 1)==0){
				/*printf("\n Line: %s \n ", line);*/
				line[i+1]='\0';
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

char* getcharstillchar(char* text, int start, char c){
	char* line;
	int i;
	int end = indexof(&c, text, start);
	int length= end-start;
	line = (char*) malloc(length);
	for (i=0;i<length; i++){
		strncpy(&(line[i]), &(text[i+start]), 1);
	}
	strncpy(&(line[i]), "\0", 1);

	return line;
}

int compareignore(char* s1, char* s2){

	char s1c;
	  do
	    {
	      s1c = *s1;
	      if (s1c == '\n')
	          s1c = 0;
	      if (s1c != *s2)
	          return 1;
	      s1++;
	      s2++;
	    } while (s1c);
	  return 0;
}

int indexof (char* c, char* line, int start){
	for (;start<strlen(line); start++){
		if (strncmp(&(line[start]), c, 1)==0){
			return start;
		}
	}
	return -1;
}

void printlabels(){
	int i = 0;
	for (i=0; i< labelindex; i++){
		printf ("Label %s address %d\n" , symboltable[i].name, symboltable[i].value);
	}
}
void initwords(){
	int i,j;
	for (j=0; j< 256; j++){
		for (i=0; i<10;i++){
			memory[j].value[i]=0;
		}

	}
	for (j=0; j< 256; j++){
		for (i=0; i<10;i++){
			datamemory[j].value[i]=0;
		}
	}
}
