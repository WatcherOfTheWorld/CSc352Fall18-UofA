/* 
* File: sumLine.c
* Author: Feiran Yang
* Purpose: this program print out the sum of a input line
* exit until reach the EOF.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void){
	char* line = NULL;
	size_t len = 0;
	int returnCode = 0;

	// read in a new line
	while(getline(&line, &len, stdin) != EOF){
		int count = 0;
		int notErr;
		int empty = 1;
		int thisNum;
		int offset;
		// read in next int
		while((notErr = sscanf(line,"%d%n",&thisNum, &offset))>0){
			empty = 0;
			if(thisNum <1){
				notErr = 0;
				break;
			}
			line += offset;
			// increamting count
			count+=thisNum;

		}
		if(notErr && !empty){
			printf("%d\n", count);
		}else{
			fprintf(stderr, "there is an err\n");
			returnCode = 1;
		}
	}
	return returnCode;
}