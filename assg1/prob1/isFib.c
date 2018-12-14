/*
* File: sumDigits.c
* Author: Feiran Yang
* Purpose: allows user input numbers and print 
* a message to tell user whether the input number is 
* fib or not
*/ 
#include <stdio.h>
int main(void){
	int n;
	int err;
	// store return code of scanf
    while((err = scanf("%d",&n))>0){
    	if(n<1){
    		fprintf(stderr,"ERROR: %d is not a positive integer\n",n);
    		continue;
    	}

    	int i = 0;
    	int j = 1;
    	while((i+j) != n && (i+j)<n){
    		int tep = j;
    		j = i+j;
    		i = tep;
    	}
    	// if i puls j == n, n is fib
    	if((i+j)==n){
    		printf("%d is fib\n",n);
    	// otherwise is not fib
    	}else{
    		printf("%d is not fib\n",n);
    	}
    }
    
    // if return code of scanf is 
    if(err == 0){
    	fprintf(stderr,"Non-integer input\n");
    	return 1;
    }

    return 0;
}