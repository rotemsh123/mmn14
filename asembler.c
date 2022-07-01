#include "mmn14.h"
int linetype(char* line)
{
	int ind=0;
	ignorewhitechar(line,ind);
	if(line[ind]=='\n'){
		return 1;
	}
	else if(line[ind]==';'){
		return 2;
	}
	return 3;
}
int orderornot(char* line){
	int ind=0;
	ignorewhitechar(line,ind);
	return ind;

}
