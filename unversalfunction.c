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
