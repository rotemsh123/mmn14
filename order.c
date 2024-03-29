/*
 * order.c
 *
 *  Created on: Jun 28, 2022
 *      Author: user
 */

#include "mmn14.h"


/*
 * 0-1: code type: A, R, E
 * 2-3 op address code target
 * 4-5 op address code source
 * 6-9 operation code
 */
void getword(int codeType, int sourceaddresscode, int targetaddresscode, int ordercode, WORD* w){

	int i;

	/*
	 * code type 0-1
	 */
	(w[0].value)[0]=0;
	(w[0].value)[1]=0;

	/*
	 * target 2-3
	 */
	for (i=2; i<=3; i++){
		(w[0].value)[i]=targetaddresscode % 2;
		targetaddresscode /=2;
	}
	/*
	 * source 4-5
	 */

	for (i=4; i<=5; i++){
		(w[0].value)[i]=sourceaddresscode % 2;
		sourceaddresscode /=2;
	}
	/*
	 * operation 6-9
	 */
	for (i=6; i<=9; i++){
		(w[0].value)[i]=ordercode %2;
		ordercode /=2;
	}

}

/*
 * this function initialize the first 2 digit in a word
 * if the lable exists in the same file - those 2 digit will be 00 and represent A
 * if the lable exists in the entry list - those 2 digit will be 10 and represent R
 * if the lable exists in the external list - those 2 digit will be 01 and represent E
 */
void initthefirst2digitinword(WORD *w, int linenumber, char* labelname){
	int i;
	int structexist=0;
	for (i=0;i<labelindex; i++){
		if (VERBOSS > 2){
			printf("Label: '%s'\n",entry[i] );
		}
		if (strcmp(symboltable[i].name, labelname) == 0){
			w[0].value[0] = 0;
			w[0].value[1] = 1;
			structexist = 1;
			break;
		}
	}
	for (i=0;i<externindex; i++){
		if (strcmp((external[i].name), labelname) == 0){
			external[i].addresses[external[i].numofaddresses] = IC;
			external[i].numofaddresses++;
			w[0].value[0] = 1;
			w[0].value[1] = 0;
			structexist =1;
			break;
		}
	}
	/*meaning I couldnt find the struct in the file*/
	if (structexist==0){
		if (VERBOSS > 0){
			printf("ERROR in line %d: Illegal struct name: %s \n", linenumber, labelname);
		}
		ERROR = 1;
		w[0].value[0] = 0;
		w[0].value[1] = 0;
	}
}
/*
 * this function initiallize the words when the argument is struct
 * first word is the struct adress
 * second word is the struct field number
 */
void initializestructwords(char* arg, int linenumber){
	int i;
	int index;
	WORD w, w2;

	/*struct with 2 words. first is the lable address and second is 1 or 2 (first field or second in the struct)*/
	char* labelname = getcharstillchar(arg, 0, '.');
	int lableadress = getlabeladdress(labelname);

	initthefirst2digitinword(&w, linenumber, labelname);

	for (i=2; i<=9; i++){
		w.value[i] = lableadress%2;
		lableadress/=2;
	}
	memory[IC] = w;
	IC++;

	index = indexof(".", arg, 0)+1;

	for (i=0; i<=9; i++){
		w2.value[i]=0;
	}
	if (strncmp(&arg[index] , "1", 1)==0){
		w2.value[2]=1;
	}
	else if (strncmp(&arg[index] , "2", 1)==0 ){
		w2.value[3]=1;
	}
	else{
		if (VERBOSS > 0){
			printf("ERROR in line %d: Illegal struct field (must be 1 or 2): %s \n", linenumber, &arg[index]);
		}
		ERROR = 1;
	}
	if ((strncmp(&arg[index+1] , " ", 1)!=0) &&
			(strncmp(&arg[index+1] , "\t", 1)!=0)&&
			(strncmp(&arg[index+1] , ",", 1)!=0) &&
			(strncmp(&arg[index+1] , "\0", 1)!=0) &&
			(strncmp(&arg[index+1] , "\n", 1)!=0)){
		if (VERBOSS > 0){
			printf("ERROR in line %d: After Struct must have one of the following chars: (' ', ',', \t, \n, ''). next char is: \"%s\" \n", linenumber, &arg[index+1]);
		}
		ERROR = 1;
	}




	memory[IC] = w2;
	IC++;
}

/*
 * initialize the word when the argumrnt is direct number
 */
void initializesimmediateword(char* arg, int linenumber){
	int index;
	ignorewhitechar(arg, 0);
	index = indexof("#", arg, 0)+1;
	inttoword(&arg[index], linenumber, &memory[IC],0);
	IC++;
}

/*
 * initialize a WORD when the value is direct - name of a label
 */
void initializesdirectword(char* labelname, int linenumber){
	WORD w;
	int j;
	int lableadress = getlabeladdress(labelname);

	for (j=0; j<=9; j++){
		w.value[j] = 0;
	}
	initthefirst2digitinword(&w, linenumber, labelname);
	j=2;
	while (lableadress != 0) {
		w.value[j] = lableadress % 2;
		lableadress /= 2;
		j++;
	}
	memory[IC] = w;
	IC++;
}

/*
 * initiaize the word with the register number
 * if register in source, put it in the ints between 6-9
 * if register is target - put it in the ints between 2-5
 */
void initializesregisterword(char* arg, int linenumber, WORD* w, int sourceortarget){
	int d, i,j;

	d = arg[1]-'0';
	if (sourceortarget == 1){
		for (i=0; i<=9; i++){
			w[0].value[i] = 0;
		}
		j = 2; /* source register number should be between 6-9*/
	}
	else{
		j = 6;
	}
	while (d != 0) {
		w[0].value[j] = d % 2;
		d /= 2;
		j++;
	}
}

/*
 * 0 - immediate - start with #
 * 1 - direct - name of label
 * 2 - struct
 * 3 - register
 */

void handleArg(char* arg, int linenumber, int sourceortarget){
	int argaddresscode;
	argaddresscode = getaddresscode(arg);
	if ((argaddresscode == 0)){
		initializesimmediateword(arg, linenumber);
	}
	else if ((argaddresscode == 1)){
		initializesdirectword(arg, linenumber);
	}
	else if ((argaddresscode == 2)){
		initializestructwords(arg, linenumber);
	}
	else if ((argaddresscode == 3)){
		initializesregisterword(arg, linenumber, &memory[IC], sourceortarget);
		IC++;

	}

}
/*
 * handle all orders with 1 pram: not, clr, inc, dec, jmp, bne, get, prn, jsr
 * Arguments:
 * 1. linenumber: line number - to use in error logs
 * 2. curline: pointer to the current line
 * 3. ordercode: which order to run
 * 4. order: pointer to the order tp [rint in case of error
 * 5. index: index of the char after the order in the line
 */

void handle1param(int linenumber, char* curline, int ordercode, char* order, int index){
	char* arg1;
	int arg1addresscode;
	/*
	 * only prn - (opcode 12) can has target address code =0
	 */
	if ((indexof(",", curline, index) !=-1) || (hassomethingtillendofline(curline, index) != 0)){
		if (VERBOSS > 0){
			printf("ERROR in line %d: Too many operands for order: %s \n", linenumber, order);
		}
		ERROR = 1;
		return;
	}
	arg1 = getcharstillchar(curline, index, '\n');
	arg1addresscode = getaddresscode(arg1);
	if ((arg1addresscode==0) && (ordercode!=12) ){
		if (VERBOSS > 0){
			printf("ERROR in line %d: Order %s doesn't support direct address code for the target \n", linenumber, order);
		}
		ERROR = 1;
	}

	getword(0, 0, arg1addresscode, ordercode, &memory[IC]);
	IC++;
	/*
	 * add the arg to the memory
	 * */
	handleArg(arg1, linenumber, 0);

	if (VERBOSS > 2){
		char* wts;
		char* wtsw;
		printf("Word is %d-%d-%d-%d\n", ordercode, 0, arg1addresscode, 0);
		wts = WORDtostring(memory[IC]);
		printf("The word is: %s\n", wts);
		wtsw = WORDtostringwithminus(memory[IC]);
		printf("The word with minus is: %s\n", wtsw);
		free (wts);
		free (wtsw);
	}


}


/*
 * handle all orders with 2 prams: mov, cmp, add, sub, lea
 * Arguments:
 * 1. linenumber: line number - to use in error logs
 * 2. curline: pointer to the current line
 * 3. ordercode: which order to run
 * 4. order: pointer to the order tp [rint in case of error
 * 5. index: index of the char after the order in the line
 */
void handle2param(int linenumber, char* curline, int ordercode, char* order, int index){
	char* arg1;
	char* arg2;
	int arg1addresscode;
	int arg2addresscode;

	if (indexof(",", curline, index) ==-1){
		if (VERBOSS > 0){
			printf("ERROR in line %d: Missing operands for order: %s \n", linenumber, order);
		}
		ERROR = 1;
		return;
	}
	arg1 = getcharstillchar(curline, index, ',');
	index = indexof(",", curline, index) +1;
	index = ignorewhitechar(curline, index);
	arg2 = getcharstillchar(curline, index, '\n');

	if ((indexof(",", arg2, 0) !=-1)){
		if (VERBOSS > 0){
			printf("ERROR in line %d: Too many operands for order: %s \n", linenumber, order);
		}
		ERROR = 1;
		return;
	}

	arg1addresscode = getaddresscode(arg1);
	arg2addresscode = getaddresscode(arg2);


	/*
	 * only cmp - (opcode 1) can has target address code =0
	 */
	if ((arg1addresscode==0) && (ordercode!=1) ){
		if (VERBOSS > 0){
			printf("ERROR in line %d: Order %s doesn't support direct address code for the target \n", linenumber, order);
		}
		ERROR = 1;
	}
	getword(0, arg1addresscode, arg2addresscode, ordercode, &memory[IC]);

	if (VERBOSS > 2){
		printf("Word is %d-%d-%d-%d\n", ordercode, arg2addresscode, arg1addresscode, 0);
		printf("The word in %d is: %s\n", IC, WORDtostring(memory[IC]));
		printf("The word with minus is: %s\n", WORDtostringwithminus(memory[IC]));
	}

	IC++;

	/*
	 * add the arg to the memory
	 * */
	if ((arg1addresscode == 3) && (arg2addresscode == 3)){
		/* if both registers - they will share 1 word*/
		WORD w;
		initializesregisterword(arg2, linenumber, &w, 1);
		initializesregisterword(arg1, linenumber, &w, 0);
		memory[IC] = w;
		IC++;
	}
	else{
		handleArg(arg1, linenumber, 0);
		handleArg(arg2, linenumber, 1);
	}

}

/*
 * this function handle a line when the line contain order
 */
void handleorder(int linenumber, char* curline, int index){
	char* order;
	int ordercode;
	index = ignorewhitechar(curline, index);
	if (VERBOSS > 2){
		printf("order line: %s", &(curline[index]));
	}
	if (indexof(" ", curline, index)!=-1){
		order = getcharstillchar(curline, index, ' ');
	}
	else{
		order = getcharstillchar(curline, index, '\n');
	}

	ordercode = ordertrans(order);
	if (ordercode == -1){
		if (VERBOSS > 0){
			printf("ERROR in line %d: Order %s is not supported \n", linenumber, order);
		}
		ERROR = 1;
		return;
	}

	/*order without params*/
	if (ordercode >= 14){
		if (VERBOSS > 2){
			printf("order (with no param) is %s code is: %d\n", order, ordercode);
		}
		getword(0, 0, 0, ordercode, &memory[IC]);
		IC++;
		return;
	}

	index = indexof(" ", curline, index);

	if (index == -1){
		if (VERBOSS > 0){
			printf("ERROR in line %d: Missing first operand for order: %s \n", linenumber, order);
		}
		ERROR = 1;
		return;
	}

	index = ignorewhitechar(curline, index);

	/*order with 1 param*/
	if (ordercode ==4 || ordercode == 5 || (ordercode >= 7 && ordercode <= 13) ){
		handle1param(linenumber, curline, ordercode, order, index);
		return;
	}

	/*order with 2 params*/
	if (ordercode <=3 || ordercode == 6){
		handle2param(linenumber, curline, ordercode, order, index);
		return;
	}

	if (VERBOSS > 0){
		printf("ERROR in line %d: Illegal order: %s \n", linenumber, order);
	}
	ERROR = 1;

}

/*
 * round 1 of order
 *
 */


/*
 * This function is for round 1 of the assembler - calculate the number of WORD needed for 1 param order
 */
void handle1paramLabel(int linenumber, char* curline, int ordercode, char* order, int index){
	char* arg1;
	int arg1addresscode;
	int numberofwords;

	arg1 = getcharstillchar(curline, index, '\n');
	arg1addresscode = getaddresscode(arg1);


	if (islabelline == 1){
		symboltable[labelindex].value = IC;
		symboltable[labelindex].DI = 0;
		labelindex++;
		islabelline=0;
	}
	/*
	 * calculate number of words we need
	 */
	numberofwords = 1;
	if ((arg1addresscode == 2)){
		numberofwords+=2;
	}
	/* if order address code is 0,1 or 3*/
	else{
		numberofwords++;
	}
	IC += numberofwords;


}

/*
 * This function is for round 1 of the assembler - calculate the number of WORD needed for 2 params order
 */

void handle2paramLabel(int linenumber, char* curline, int ordercode, char* order, int index){
	char* arg1;
	char* arg2;
	int arg1addresscode;
	int arg2addresscode;
	int numberofwords;

	arg1 = getcharstillchar(curline, index, ',');
	index = indexof(",", curline, index) +1;
	index = ignorewhitechar(curline, index);
	arg2 = getcharstillchar(curline, index, '\n');

	arg1addresscode = getaddresscode(arg1);
	arg2addresscode = getaddresscode(arg2);


	if (islabelline == 1){
		symboltable[labelindex].value = IC;
		symboltable[labelindex].DI = 0;
		labelindex++;
		islabelline=0;
	}
	/*
	 * calculate number of words we need
	 */
	numberofwords = 1;
	if ((arg1addresscode == 3) && (arg2addresscode == 3)){
		/* if both registers - they will share 1 word*/
		numberofwords+=1;
	}
	else{
		if ((arg1addresscode==0) || (arg1addresscode==1)|| (arg1addresscode==3)){
			numberofwords+=1;
		}
		if (arg1addresscode==2){
			numberofwords+=2;
		}
		if ((arg2addresscode==0) || (arg2addresscode==1) || (arg2addresscode==3)){
			numberofwords+=1;
		}
		if (arg2addresscode==2){
			numberofwords+=2;
		}
	}
	IC += numberofwords;

}

/*
 * This function is for round 1 of the assembler
 */
void handleorderLabel(int linenumber, char* curline, int index){
	char* order;
	int ordercode;
	index = ignorewhitechar(curline, index);
	if (VERBOSS > 2){
		printf("order line: %s", &(curline[index]));
	}
	if (indexof(" ", curline, index)!=-1){
		order = getcharstillchar(curline, index, ' ');
	}
	else{
		order = getcharstillchar(curline, index, '\n');
	}

	ordercode = ordertrans(order);

	/*order without params*/
	if (ordercode >= 14){
		if (islabelline == 1){
			symboltable[labelindex].value = IC;
			symboltable[labelindex].DI = 0;
			labelindex++;
			islabelline=0;
		}
		IC++;
		return;
	}

	index = indexof(" ", curline, index);
	index = ignorewhitechar(curline, index);


	/*order with 1 param*/
	if (ordercode ==4 || ordercode == 5 || (ordercode >= 7 && ordercode <= 13) ){
		handle1paramLabel(linenumber, curline, ordercode, order, index);
		return;
	}

	/*order with 2 params*/
	if (ordercode <=3 || ordercode == 6){
		handle2paramLabel(linenumber, curline, ordercode, order, index);
		return;
	}


}
