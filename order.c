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


	// order with 2 params
	if (ordercode <=3 || ordercode == 6){
		printf("order (with 2 params) is %s code is: %d\n", order, ordercode);
		return;
	}
	// order with 1 param
	if (ordercode ==4 || ordercode == 5 || (ordercode >= 7 && ordercode <= 13) ){
		printf("order (with 1 param) is %s code is: %d\n", order, ordercode);
		return;
	}
	//order without params
	if (ordercode >= 14){
		printf("order (with no param) is %s code is: %d\n", order, ordercode);
		return;
	}
	printf("Error in line %d: Illegal order: %s \n", linenumber, order);

}
