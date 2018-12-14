/*
* File: sumDigits.c
* Author: Feiran Yang
* Purpose: Print out the sum of input string on screen
*/ 
#include <stdio.h>

int main(void){
	int n;
	int err;
	while((err = scanf("%d",&n))>0){
		if(n < 1){
			fprintf(stderr,"ERROR: %d is not a positive integer\n",n);
    		continue;
		}

		int count = 0;
		int i = 0;
		char str[15];
		sprintf(str, "%d", n);
		while(i<15){
			if(((int)str[i])>='0'&& ((int)str[i])<='9'){
				count += ((int)str[i] - '0');
				// clear previous data after adding digits
				str[i] = '0';
			}
			i++;
		} 
		printf("%d\n", count);

	}
	    if(err == 0){
    	fprintf(stderr,"Non-integer input\n");
    	return 1;
    }
    return 0;
}

