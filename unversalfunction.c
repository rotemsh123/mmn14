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

int getlabeladdress(char* labelname){
	int i;
	for (i=0; i< labelindex; i++){
		if (strcmp(symboltable[i].name, labelname)==0){
			return (symboltable[i]).value;
		}
	}
	printf ("ERROR. label: '%s' doesn't exist\n", labelname);
	return 0;
}

int stringtoint(char *string, int linenumber) {
	int result;
	int puiss;
	int index=0, length;
	index=ignorewhitechar(string,index);

	length = strlen(string);
	result = 0;
	puiss = 1;
	while (('-' == string[index]) || (string[index] == '+')) {
		if (string[index] == '-')
			puiss = puiss * -1;
		index++;
	}
	while (index<length) {
		if ((string[index] >= '0') && (string[index] <= '9')){
			result = (result * 10) + (string[index] - '0');
			index++;
		}
		else{
			printf("ERROR in line %d: Expecting number and got char: %c\n",linenumber, string[index]);
			break;
		}
	}

	return (result * puiss);
}

/**
 * convert int into word
 * if dataordirect ==0  menaing it is a data line (first 2 digit are used)
 * dataordirect !=0 meaning it is direct (first 2 digit should be 0)
 */
void inttoword(char* string, int linenumber, WORD* w, int dataordirect){
	int d;
	int negative=0, i=0, j=0, k=0;
	negative = 0;
	d = stringtoint(string, linenumber);

	printf("char is: %s and int is: %d\n", string, d);

	if (dataordirect == 0){
		i = 2;
		j = 2;
		k = 2;
		w[0].value[0] = 0;
		w[0].value[1] = 0;
	}

	if (d<0){
		d*=-1;
		negative = 1;
	}
	while (d != 0) {
		w[0].value[i] = d % 2;
		d /= 2;
		i++;
	}
	/*if number is negative, I switch each 0 to 1 and than add 1*/
	if (negative == 1){
		for (; j<10; j++){
			if(w[0].value[j] == 0){
				w[0].value[j] = 1;
			}
			else {
				w[0].value[j] = 0;
			}
		}
		/*now should add 1*/
		for (; k<10; k++){
			if(w[0].value[k] == 0){
				w[0].value[k] = 1;
				break;
			}
			else{
				w[0].value[k] = 0;
			}
		}
	}

}
