#include "mmn14.h"

int ignorewhitechar(char* line,int ind) {

	while(strncmp(&(line[ind]), " ",1) == 0 || strncmp(&(line[ind]), "\t",1) == 0) {
		ind++;
	}
	return ind;
}

/*
 * this function read line for a file
 */
char* readline(FILE* f, int linenumber) {
	char* line;
	int i=0;
	line=(char*)malloc(80);

	while (1){
		if ((fscanf(f, "%c",&(line[i]))==1) && (i <80)){
			if (strncmp(&(line[i]),"\n", 1)==0){
				/*printf("\n Line: %s \n ", line);*/
				line[i+1]='\0';
				return line;
			}
			i++;
			if (i>=80){
				if (VERBOSS > 0){
					printf ("ERROR in line %d: Line length should not be longer than 80\n", linenumber);
					ERROR = 1;
				}
				return line;
			}
		}
		else{
			/*printf ("LAST LINE");*/
			lastLine = 1;
			/*printf("\n Line: %s \n ", line);*/
			return line;
		}
	}

}

/*
 * this function gets a char, scan text and return the text till the first appearance of this char
 */
char* getcharstillchar(char* text, int start, char c){
	char* line;
	int length;
	int i;
	int end = indexof(&c, text, start);
	if (end <0){
		if (VERBOSS > 2){
			printf ("char %c doesnt exist in text %s\n" , c, text);
		}
		return text;
	}
	length= end-start;
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

/*
 * this function return the index of chanr is a text starting from the index provided
 */
int indexof (char* c, char* line, int start){
	for (;start<strlen(line); start++){
		if (strncmp(&(line[start]), c, 1)==0){
			return start;
		}
	}
	return -1;
}

/*
 * This function checks if there is something in the line after space
 */
int hassomethingtillendofline(char* line, int start){
	int index;
	index = indexof(" ", line, start);
	if (index == -1){
		return 0;
	}
	index = ignorewhitechar(line, index);
	if(line[index]=='\n'){
		return 0;
	}
	return 1;

}


void printlabels(){
	int i = 0;
	for (i=0; i< labelindex; i++){
		if (VERBOSS > 2){
			printf ("Label %s address %d\n" , symboltable[i].name, symboltable[i].value);
		}
	}
}

/*
 * this function initialize all 256 words in the memory and 256 words in data memory with zeros
 */
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

/*
 * this function scans the symbole table and return the index of the label name in argument
 */
int getlabeladdress(char* labelname){
	int i;
	for (i=0; i< labelindex; i++){
		if (strcmp(symboltable[i].name, labelname)==0){
			return (symboltable[i]).value;
		}
	}
	if (VERBOSS > 2){
		printf ("Label: '%s' doesn't exist as internal label, we will treat it as entern or external\n", labelname);
	}
	return 0;
}

/*
 * this function stranslate number which appear as string to int
 */
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
			if (VERBOSS > 0){
				printf("ERROR in line %d: Expecting number and got char: %c\n",linenumber, string[index]);
			}
			ERROR = 1;
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

	if (VERBOSS > 2){
		printf("char is: %s and int is: %d\n", string, d);
	}

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
