#include "mmn14.h"
int power(int a,int b);
char base32[32] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
char final[2];
char opcode[16][3] = {{'m','o','v'},{'c','m','p'}, {'a','d','d'}
, {'s','u','b'},{'n','o','t'}, {'c','l','r'}, {'l','e','a'}, {'i','n','c'}, {'d','e','c'}, {'j','m','p'},
{'b','n','e'}, {'g','e','t'}, {'p','r','n'}, {'j','s','r'}, {'r','t','s'}, {'h','l','t' }};

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
		if(strcmp(opcode[i],order)==0){
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







































