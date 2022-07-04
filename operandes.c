
#include "mmn14.h"

/*
 * 0 - immediate - start with #
 * 1 - direct - name of label
 * 2 - struct
 * 3 - register
 */

int getaddresscode(char* arg){
	if (strncmp(&(arg[0]), "#" , 1) ==0){
		return 0;
	}

	if (indexof(".", arg, 0) != -1){
		return 2;
	}

	if ((strcmp(arg, "r0") ==0)||(strcmp(arg, "r1") ==0)||(strcmp(arg, "r2") ==0)||(strcmp(arg, "r3") ==0)||
		(strcmp(arg, "r4") ==0)||(strcmp(arg, "r5") ==0)||(strcmp(arg, "r6") ==0)||(strcmp(arg, "r7") ==0)){
		return 3;
	}

	/*
	 * if it is not immediate, direct or struct, I assume it is label and
	 * if I'll not find the label eventually, it will become error
	 */
	return 1;




}
