/* 
* File cipher.c
* Author: Feiran Yang
* Purpose: this program rotate the char in string in by the first 
* in put value
* final result will be output to stdout
*/
#include <stdio.h>
#include <ctype.h>

char rotation(char, int, int);
int editString(char[], char[], int);

int main(void){
	int err = 0;
	int isErr = 0;
	int rotate;
	char string[65];
	char editedString[65];
	if(scanf("%d", &rotate) > 0 ){
		while(scanf("%64s", string) != EOF){
			// edit the string in editString method
			// and record the value returned by this method 
			// to check if there is invalid char
			err = editString(string, editedString, rotate);
			if(err){	// if there is, output msg to stderr
				fprintf(stderr, "input string in wrong format\n");
				isErr = 1;
			}else{
				printf("%s\n",editedString);
			}
		}
	}else{
		return 1;
	}
	return isErr;
}

/*
* Rotate one char by [rotate] times
* return a retated char back
*/
char rotation(char oneChar, int rotate, int isCap){
	int newChar = (int)oneChar + rotate;
	// make sure the caplizition wont change
	char thisA = 'a';
	char thisZ = 'z';
	if(isCap){
		thisA = 'A';
		thisZ = 'Z';
	}
	while(newChar<thisA){
		newChar = newChar + 26;
	}
	while(newChar>thisZ){
		newChar = newChar-(26);
	}
	return newChar;
}

/*
* edit string, if the char in string is number, it will
* be remian unchanged, if the char is not numer or alphabet
* return 1 to show call there is an err.
* otherwise rotate the char by [rotate].
*/
int editString(char string[], char edit[], int rotate){
	int i;
	for(i = 0; i < 64; i++){
		char currChar = string[i];
		if(string[i] == 0){
			break;
		}else if(!isalnum(currChar)){
			return 1;
		}else if(isdigit(currChar)){
			edit[i] = string[i];
		}else if(isalpha(currChar)){
			int isCap = 0;
			// check if the char is in upper case
			if(currChar<'a'){
				isCap = 1;
			}
			edit[i] = rotation(currChar,rotate,isCap);
		}
	}
	edit[i] = 0;
	return 0;
}
