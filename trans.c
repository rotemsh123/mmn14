#include "mmn14.h"
int power(int a,int b);
char base32[32] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
char* opcode[16] = {"mov","cmp", "add", "sub","not", "clr", "lea", "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};

char* trans32(int number){
	char* final;
	int i;
	final = (char*)malloc(3);
	for (i=1; i>=0; i--){
		final[i] = base32[number%32];
		number/=32;
	}
	final[2] = '\0';
	return final;
}

int WORDtoInt(WORD w){
	int i;
	int number=0;
	for(i=0; i< 10; i++){
		number = number + (w.value[i])*power(2,(i));
	}
	return number;
}

int ordertrans(char* order){
	int i = 0;
	for (i=0; i< 16; i++){
		char * ordername = opcode[i];
		if(strcmp(ordername,order)==0){
			return i;
		}
	}
	return -1;
}

int power(int a,int b){
	int i;
	if(b==0){
		return 1;
	}
	if(b==1){
		return a;
	}
	for(i=1;i<b;i++){
		a=a*2;
	}
	return a;
}

char* WORDtostring(WORD word){
	char* wordinstring;
	int i=0;
	wordinstring = (char*)malloc(11);
	for (;i<=9; i++){
		wordinstring[i] = (word.value)[9-i]+'0';
	}
	wordinstring[10] = '\0';
	return wordinstring;
}


char* WORDtostringwithminus(WORD word){
	char* wordinstring;
	int i;
	wordinstring = (char*)malloc(13);
	for (i=0;i<=3; i++){
		wordinstring[i] = (word.value)[9-i]+'0';
	}
	wordinstring[4] = '-';
	for (i=5;i<=6; i++){
		wordinstring[i] = (word.value)[10-i]+'0';
	}
	wordinstring[7] = '-';
	for (i=8;i<=9; i++){
		wordinstring[i] = (word.value)[11-i]+'0';
	}
	wordinstring[10] = '-';
	for (i=11;i<=12; i++){
		wordinstring[i] = (word.value)[12-i]+'0';
	}
	wordinstring[13] = '\0';
	return wordinstring;
}





































