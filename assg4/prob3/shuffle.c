/* 
* File: shuffle.c
* Author: Feiran Yang
* Purpose: this program 
*/

#include <stdio.h>
#include <ctype.h>

int checkString(char*);

int main(void){
	char* line1 = NULL;
	size_t len1 = 0;
	char* line2 = NULL;
	size_t len2 = 0;
	int offset1, offset2;
	int num1, num2;

	// readin the first line
	if(getline(&line1, &len1, stdin) != EOF){
		if(checkString(line1)){
			fprintf(stderr, "Error input\n");
			return 1;
		}
		// readin the seconed line
		getline(&line2, &len2, stdin);
		if(line2 != NULL){
			if(checkString(line2)){
				fprintf(stderr, "Error input\n");
				return 1;
			}
		}else{
			*line2 = 0;
		}

		// print the integers in new order 
		int result1 = sscanf(line1,"%d%n",&num1,&offset1);
		int result2 = sscanf(line2, "%d%n", &num2, &offset2);
		while(result1>0 || result2>0){
			if(result1>0){
				printf("%d\n", num1);
				line1+= offset1;
			}
			if(result2>0){
				printf("%d\n", num2);
				line2 += offset2;
			}
			result1 = sscanf(line1,"%d%n",&num1,&offset1);
			result2 = sscanf(line2, "%d%n", &num2, &offset2);
		}
	}

	return 0;
}


/* 
* check if input line is legit.
* a legit line should only contians space, '-' and numbers
* return 0 if is legit, otherwise return 1;
*/
int checkString(char* line){
	int i = 0;
	while(*(line+i)!= 0){
		// check if curr char is a number
		if((*(line+i)>=(int)'0'&&*(line+i)<=(int)'9')){
			// do nothing
		// if curr char is '-', check if the pre char is space
		// and the next char is a number
		}else if(*(line+i)==(int)'-'){
			if(i == 0 || isspace(*(line+i-1))){
				if(isspace(*(line+i+1))){
					return 1;
				}
			}else{
				return 1;
			}
		// check is curr char is space.
		}else if(isspace(*(line+i))){
			// do nothing
		}else{
			return 1;
		}
		i++;
	}
	return 0;
}
