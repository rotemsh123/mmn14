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
 * handle all orders with 1 pram: not, clr, inc, dec, jmp, bne, get, prn, jsr
 * the only argument is the target operand
 */

void handle1param(int linenumber, char* curline, int ordercode, char* order, int index){
	char* arg1;
	int arg1addresscode;
	int numberofwords;
	/*
	 * only prn - (opcode 12) can has target address code =0
	 */
	if (indexof(",", curline, index) !=-1){
		printf("ERROR in line %d: Too many operands for order: %s \n", linenumber, order);
		return;
	}
	arg1 = getcharstillchar(curline, index, '\n');
	arg1addresscode = getaddresscode(arg1);
	if ((arg1addresscode==0) && (ordercode!=12) ){
		printf("ERROR in line %d: order %s doesn't support direct address code for the target \n", linenumber, order);
	}

	getword(0, 0, arg1addresscode, ordercode, &memory[IC]);

	printf("Word is %d-%d-%d-%d\n", ordercode, 0, arg1addresscode, 0);
	printf("The word is: %s\n", WORDtostring(memory[IC]));
	printf("The word with minus is: %s\n", WORDtostringwithminus(memory[IC]));


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


void handle2param(int linenumber, char* curline, int ordercode, char* order, int index){
	char* arg1;
	char* arg2;
	int arg1addresscode;
	int arg2addresscode;
	int numberofwords;

	if (indexof(",", curline, index) ==-1){
		printf("ERROR in line %d: missing operands for order: %s \n", linenumber, order);
		return;
	}
	arg1 = getcharstillchar(curline, index, ',');
	index = indexof(",", curline, index) +1;
	index = ignorewhitechar(curline, index);
	arg2 = getcharstillchar(curline, index, '\n');

	arg1addresscode = getaddresscode(arg1);
	arg2addresscode = getaddresscode(arg2);

	/*
	 * only cmp - (opcode 1) can has target address code =0
	 */
	if ((arg1addresscode==0) && (ordercode!=1) ){
		printf("ERROR in line %d: order %s doesn't support direct address code for the target \n", linenumber, order);
	}
	getword(0, arg1addresscode, arg2addresscode, ordercode, &memory[IC]);

	printf("Word is %d-%d-%d-%d\n", ordercode, arg2addresscode, arg1addresscode, 0);
	printf("The word in %d is: %s\n", IC, WORDtostring(memory[IC]));
	printf("The word with minus is: %s\n", WORDtostringwithminus(memory[IC]));


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

void handleorder(int linenumber, char* curline, int index){
	char* order;
	int ordercode;
	printf("order line: %s", &(curline[index]));
	index = ignorewhitechar(curline, index);
	if (indexof(" ", curline, index)!=-1){
		order = getcharstillchar(curline, index, ' ');
	}
	else{
		order = getcharstillchar(curline, index, '\n');
	}

	ordercode = ordertrans(order);

	/*order without params*/
	if (ordercode >= 14){
		printf("order (with no param) is %s code is: %d\n", order, ordercode);
		getword(0, 0, 0, ordercode,&memory[IC]);

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

	if (index == -1){
		printf("ERROR in line %d: missing first operand for order: %s \n", linenumber, order);
		return;
	}


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


	printf("ERROR in line %d: Illegal order: %s \n", linenumber, order);

}
