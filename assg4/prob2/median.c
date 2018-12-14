/* 
* File median.c
* Author: Feiran Yang
* Purpose: this program read in number is one line, then compute the 
* median of those numbers
*/

#include <stdio.h>
#include <stdlib.h>

int* sort(int*,int);
void countMedian(int*, int);

int main(void){
	int size;
	int *integers;
	if(scanf("%d",&size)>0){
		// check input value
		if(size<1){
			fprintf(stderr, "N is not positive\n");
			return 1;
		}else{
			// performing a dynamic memory allocation
			integers = malloc(size*sizeof(int));
			// if failed, exitn with code 1
			if(integers == NULL){
				fprintf(stderr, "out of memory\n");
				return 1;
			}
			int i;
			// read in [size] intgers
			for(i = 0; i<size;i++){
				if(scanf("%d",(integers+i))<1){
					fprintf(stderr, "Fewer than %d int has been entered\n", size);
					free(integers);
					return 1;
				}
			}
			sort(integers,size);
			countMedian(integers,size);
			free(integers);
		}
	}else{
		fprintf(stderr, "N is not a number\n");
		return 1;
	}

	return 0;

}

/*
* sort the integers
* return the pointer of the integer array
*/
int* sort(int* integers, int size){
	int j, i;
	int temp;
	for(i = 0; i< size; i++){
		for(j = i+1; j<size;j++){
			if(*(integers+i)>*(integers+j)){
				temp = *(integers+i);
				*(integers+i) = *(integers+j);
				*(integers+j) = temp;
			}
		}
	}

	return integers;
}


/*
* comput median and print the result.
*/
void countMedian(int* integers, int size){
	float med;
	//if size is even, med is two number in middle divide 2
	if(size%2 == 0){
		med = (((float)*(integers+size/2))+((float)*(integers+size/2-1)))/2;
	}else{
		// otherwise, the number in middle.
		med = *(integers+size/2);
	}
	printf("%.1f\n", med);

}