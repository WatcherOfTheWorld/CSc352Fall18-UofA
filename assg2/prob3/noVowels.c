/*
* File: noVowels.c
* Author: Feiran Yang
* Purpose: read in series of string, the first one is pattern, 
* print out string that have the pattern with the first string 
* is all vowels has been removed
*/ 
#include <stdio.h>
#include <ctype.h>

int removeVowels(char[], char[]);

int main(void){
	char target[65];
	char string[65];
	char noValTarget[65];
	int haveTarget = 0;
	int illegalChar = 0;

	if(scanf("%64s", target) != EOF){
		haveTarget = 1;
		illegalChar = removeVowels(target, noValTarget);
	}

	// print err msg if the first string contains invalid char
	if(illegalChar){
		fprintf(stderr, "You did not enter a valid word\n");
		return 1;
	}else if(haveTarget){
		printf("%s\n", target);
	}

	while(haveTarget && scanf("%s", string) != EOF){
		char noVal[65];
		if(removeVowels(string, noVal)){
			// if there is invaild char in string, print err msg
			fprintf(stderr, "You did not enter a valid word\n");
			continue;
		}

		int isSame = 1;
		int i;
		for(i = 0; i< 64; i++){
			// check if both string reached the end of string
			if(noVal[i] == 0){
				if(noValTarget[i] != 0){
					isSame = 0;
				}
				break;
			}else if(noVal[i] != noValTarget[i]){
				isSame = 0;
			}
		}

		if(isSame){
			printf("%s\n", string);
		}
	}
	return 0;
}

/*
* remove all vowels from first array and put the result 
* back into the sec array
* return 0 if no err has occured, return 1 otherwise
*/
int removeVowels(char origal[], char target[]){
	int k = 0;
	int i;
	for(i = 0; i < 64; i++){
		char targetChar = tolower(origal[i]);
		// if reach the end of string, exit loopping
		if(targetChar == 0){
			break;
		}else if(targetChar !='a' && targetChar != 'e' &&
			targetChar != 'i' && targetChar != 'o' &&
			targetChar != 'u'){
			if(targetChar>='a'&& targetChar<='z'){
				target[k] = targetChar;
				k ++;
			}else{
				return 1;
			}
		}
	}
	// mark k as 0 as the end of string 
	target[k] = 0;
	return 0;
}
