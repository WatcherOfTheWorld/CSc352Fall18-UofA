/*
* File: sumSquares.c
* Author: Feiran Yang
* Purpose: allows user input two numbers as an 
* interval, find every number in this interval 
* that can be express as sum of two int squares
*/ 
#include <stdio.h>
#include <math.h>

int main(void){
	int inter[2];
	int err;
	int i = 0;
	// read two input to program
	while((err = scanf("%d",&inter[1]))>0 && i < 1){
		inter[i] = inter[1];
		i++;
	}

	// check err.
	if(err == 0){
		fprintf(stderr, "Non-integer input\n" );
		return 1;
	}else if(err == -1){
		fprintf(stderr, "Usage: sumSquares m n\n");
		return 1;
	}else if(inter[0] < 0 || inter[1] <0){
		fprintf(stderr, "Both range values must be positive\n");
		return 1;
	}

	// looping trough the interval
	for(i = inter[0]; i<=inter[1]; i++){
		int j;
		for(j = 1; j < i; j++){
			double upper = sqrt(j);
			double lower = sqrt(i-j);
			// check the sqrt of two number is int
			if(upper == (int) upper && lower == (int) lower){
			// if is, print the i.
				printf("%d\n", i);
				break;
			}
		}
	}


	return 0;
}


