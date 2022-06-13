/*
 * mmn14.h
 *
 *  Created on: May 31, 2022
 *      Author: user
 */

#ifndef MMN14_H_
#define MMN14_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
typedef int REGISTER[10];
typedef int WORD[10];
char* trans32(unsigned long x);
int ordertrans(char* order);
char* readline();
char* preasembler();


#endif /* MMN14_H_ */
