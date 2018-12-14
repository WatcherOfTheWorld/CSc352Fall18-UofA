/* 
* File vawels.c
* Author: Feiran Yang
* Purpose: this program checks if the input word's vowels in
* in "a,e,i,o,u" order
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int check(char[]);

int main(void){
	char string[65];
	char stringVowels[65];
	int hasErr = 0;
	while(scanf("%64s", string) != EOF){
		int err = 0;
		int i;
		int k = 0;
		for(i = 0; i< strlen(string); i++){
			// if the string contains char other than alphabet
			// print an err msg and will be exit with code 1
			if(!isalpha(string[i])){
				err = 1;
				hasErr = 1;
				fprintf(stderr, "not a word\n");
				break;
			}
			char currChar = tolower(string[i]);
			// put all vowels into a new string and remove repeated vowels
			if(currChar == 'a' || currChar == 'e'|| currChar =='i'||
				currChar == 'o' || currChar == 'u'){
				if(k == 0 || currChar != stringVowels[k-1]){
					stringVowels[k] = currChar;
					k++;
				}
			}
			stringVowels[k] = 0;
		}
		if(!err){
			// if there is not err with the curr string, print the result
			printf("%d\n", check(stringVowels));
		}

	}
	return hasErr;
}

/*
* check if a string is same before sort and after sort
* return 1 if this is true, otherwise return 0
*/
int check(char string[]){
	int l = strlen(string);
	// contains no vowels, return 1;
	if(l== 0){
		return 1;
	}
	char oldString[l+1];
	int i;
	// cp the old string to new string
	for(i= 0; i<l;i++){
		oldString[i] = string[i];
	}

	// sort the string with out non-vowels
	int j;
	char temp;
	for(i = 0; i< l; i++){
		for(j = i+1; j<l;j++){
			if(string[i]>string[j]){
				temp = string[i];
				string[i] = string[j];
				string[j] = temp;
			}
		}
	}

	// if the sorted string == orignal string
	// the vowels in the word is in right order
	for(i = 0; i<l;i++){
		if(string[i] != oldString[i]){
			return 0;
		}
	}
	return 1;
}