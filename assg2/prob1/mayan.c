/*
* File: mayan.c
* Author: Feiran Yang
* Purpose: read in strings that only contains a - z
* as base-20 numbers convert them to decimal number
*/ 
#include <stdio.h>
#include <ctype.h>

int toDes(int,int);

int main(void){
	char string[7] = "      ";// 6 spaces
	while(scanf("%6s",string) != EOF){
		int count = 0;

		int k = 0;
		int err = 0;
		int i;
		for(i = 6; i >= 0; i-- ){
			int currChar = (int)tolower(string[i]);
			if(currChar >= (int)'a' && currChar <= 't'){
				count += (currChar-(int)'a') * toDes(20, k);
				string[i] = ' ';	// clear string
				k++;
			}else if(string[i] == ' ' || string[i] == 0){
				continue;
			}else{
				// if char is not in the range of a -t, print err msg
				fprintf(stderr, "Bad input: input should be char between a - t\n");
				err =1;
				break;
			}
		}
		if(!err){
			printf("%d\n", count);
		}
	}
	return 0;
}

/*
* using recurtion to compute the result of i^degree
*/
int toDes(int i, int degree){
	// base case 1: i = 0, just return 0;
	if(i == 0){
		return 0;
	}
	// base case 2: degree = 0, return 1 
	// since for any x, x^0 = 1
	if(degree == 0){
		return 1;
	}
	return toDes(i, degree-1)*i;
}
