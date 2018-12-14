/*
* File: splitString.c
* Author: Feiran Yang
* Purpose: this program read in a string and replace all - to '\n'
* and print to stdout, repeated '-' and '-' in the end or
* start will be ignored
*/

#include <stdio.h>

int stringlen(char[]);

int main(void){
	char string[256];

	while(scanf("%255s", string) != EOF){
		int i;
		int pre = 0;
		// remove all '-' in the end of string.
		for(i= stringlen(string)-1; i>=0; i--){
			if(string[i] == '-'){
				string[i] = 0;
			}else{
				break;
			}
		}

		// print char to stdout, ignore repeated and '-' in the end
		for(i=0; i<stringlen(string); i++){
			if(string[i] != '-'){
				printf("%c", string[i]);
				pre= 1;
			}else if(pre){
				printf("\n");
				pre = 0;
			}
		}
		// if the string only contains '-', do not start a new line
		if(string[0] != 0){
			printf("\n");
		}
	}
}

/* 
* return the lenght of string(strlen())
*/
int stringlen(char string[256]){
	int i = 0;
	while(string[i]!= 0){
		i++;
	}
	return i;
}
