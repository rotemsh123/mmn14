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
WORD getword(int codeType, int targetaddresscode, int sourceaddresscode, int ordercode){
	WORD opLine;
	int i;
	/*
	 * code type 0-1
	 */
	(opLine.value)[0]=0;
	(opLine.value)[1]=0;
	/*
	 * target 2-3
	 */
	int targetaddresscodetemp = targetaddresscode;
	for (i=2; i<=3; i++){
		(opLine.value)[i]=targetaddresscodetemp % 2;
		targetaddresscodetemp /=2;
	}
	/*
	 * source 4-5
	 */
	int sourceaddresscodetemp = sourceaddresscode;
	for (i=4; i<=5; i++){
		(opLine.value)[i]=sourceaddresscodetemp % 2;
		sourceaddresscodetemp /=2;
	}
	/*
	 * operation 6-9
	 */
	int ordercodetemp = ordercode;
	for (i=6; i<=9; i++){
		(opLine.value)[i]=ordercodetemp %2;
		ordercodetemp /=2;
	}

	return opLine;
}

/*
 * handle all orders with 1 pram: not, clr, inc, dec, jmp, bne, get, prn, jsr
 * the only argument is the target operand
 */

void handle1param(int linenumber, char* curline, int ordercode, char* order, int index){
	if (indexof(",", curline, index) !=-1){
		printf("Error in line %d: Too many operands for order: %s \n", linenumber, order);
		return;
	}
	char* arg1 = getcharstillchar(curline, index, '\n');
	int arg1addresscode = getaddresscode(arg1);

	WORD opLine = getword(0, arg1addresscode, 0, ordercode);
	printf("Word is %d-%d-%d-%d\n", ordercode, 0, arg1addresscode, 0);
	printf("The word is: %s\n", WORDtostring(opLine));
	printf("The word with minus is: %s\n", WORDtostringwithminus(opLine));
	memory[IC] = &opLine;
}


void handle2param(int linenumber, char* curline, int ordercode, char* order, int index){
	if (indexof(",", curline, index) ==-1){
		printf("Error in line %d: missing operands for order: %s \n", linenumber, order);
		return;
	}
	char* arg1 = getcharstillchar(curline, index, ',');
	index = indexof(",", curline, index) +1;
	index = ignorewhitechar(curline, index);
	char* arg2 = getcharstillchar(curline, index, '\n');

	int arg1addresscode = getaddresscode(arg1);
	int arg2addresscode = getaddresscode(arg2);

	WORD opLine = getword(0, arg1addresscode, arg2addresscode, ordercode);
	printf("Word is %d-%d-%d-%d\n", ordercode, arg2addresscode, arg1addresscode, 0);
	printf("The word is: %s\n", WORDtostring(opLine));
	printf("The word with minus is: %s\n", WORDtostringwithminus(opLine));
	memory[IC] = &opLine;
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

	//order without params
	if (ordercode >= 14){
		printf("order (with no param) is %s code is: %d\n", order, ordercode);
		return;
	}

	index = indexof(" ", curline, index);
	index = ignorewhitechar(curline, index);

	if (index == -1){
		printf("Error in line %d: missing first operand for order: %s \n", linenumber, order);
		return;
	}


	// order with 1 param
	if (ordercode ==4 || ordercode == 5 || (ordercode >= 7 && ordercode <= 13) ){
		handle1param(linenumber, curline, ordercode, order, index);
		return;
	}

	// order with 2 params
	if (ordercode <=3 || ordercode == 6){
		handle2param(linenumber, curline, ordercode, order, index);
		return;
	}


	printf("Error in line %d: Illegal order: %s \n", linenumber, order);

}
