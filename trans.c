#include "mmn14.h"
int power(int a,int b);
char base32[32] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
char final[2];
char* opcode[16] = {"mov","cmp", "add", "sub","not", "clr", "lea", "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};

char* trans32(unsigned long number){
	int num[10];
	int i=0,h=0;
	int tmp=0;
	int a=0,b=0;
	for(i=9;i>=0;i--)
	{
		num[i]=number%10;
		number=number/10;
	}

	for(i=9;i>=0;i--)
	{
		tmp=tmp+num[i]*power(2,h);
		h++;
	}
	a=tmp/32;
	a=tmp-a*32;
	tmp=tmp/32;
	b=tmp/32;
	b=tmp-b*32;
	final[0]=base32[a];
	final[1]=base32[b];
	return final;
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





































