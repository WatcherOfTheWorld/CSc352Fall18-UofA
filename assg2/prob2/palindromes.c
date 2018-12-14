/*
* File: palindromes.c
* Author: Feiran Yang
* Purpose: read in strings, if string reads the same forwards 
* and backwards, print out the string
*/ 

#include <stdio.h>
#include <ctype.h>

int main(void){
	char string[65];
	while(scanf("%64s", string) != EOF){
		int end = 0;
		int i;
		for(i = 0; i < 64; i++){
			// save the index of the end of string
			if(string[i] == 0){
				end = i;
				break;
			}
		}

		int boo = 1;
		for(i = 0; i < end/2; i++){
			// check if string's forwards index char is backwards index char
			if(tolower(string[i]) != tolower(string[end-1-i])){
				boo = 0;
			}
		}

		printf("%d\n", boo);

	}

	return 0;
}
