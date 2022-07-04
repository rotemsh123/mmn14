/*
 * order.c
 *
 *  Created on: Jun 28, 2022
 *      Author: user
 */

#include "mmn14.h"

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
		printf("order (with 1 param) is %s code is: %d\n", order, ordercode);
		if (indexof(",", curline, index) !=-1){
			printf("Error in line %d: Too many operands for order: %s \n", linenumber, order);
			return;
		}
		char* arg1 = getcharstillchar(curline, index, '\n');
		printf("order %s with 1 param: arg1: %s\n", order, arg1);
		int arg1addresscode = getaddresscode(arg1);
		printf("arg1 addesscode is %d\n", arg1addresscode);
		return;
	}

	// order with 2 params
	if (ordercode <=3 || ordercode == 6){
		printf("order (with 2 params) is %s code is: %d\n", order, ordercode);
		if (indexof(",", curline, index) ==-1){
			printf("Error in line %d: missing operands for order: %s \n", linenumber, order);
			return;
		}
		char* arg1 = getcharstillchar(curline, index, ',');
		index = indexof(",", curline, index) +1;
		index = ignorewhitechar(curline, index);
		char* arg2 = getcharstillchar(curline, index, '\n');

		printf("order %s with 2 params: arg1: %s,  arg2: %s\n", order, arg1, arg2);

		int arg1addresscode = getaddresscode(arg1);
		int arg2addresscode = getaddresscode(arg2);
		printf("arg1 addesscode is %d, arg2 addresscode is %d\n", arg1addresscode, arg2addresscode);

		return;
	}


	printf("Error in line %d: Illegal order: %s \n", linenumber, order);

}
