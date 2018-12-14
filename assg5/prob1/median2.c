#include <stdio.h>
#include <stdlib.h>
/*
* Feiran Yang
* median.c
* use linked list to sort number
* out the median of inputed number
*/

struct node{
	struct node *next;
	int this;
};
void sort(struct node*);
void countMedian(struct node*, int);

int main(void){
	struct node *root = NULL;
	//struct node *curr;
	int next;
	int size = 0;
	int noInput = 1;
	int err;
	// store input to linked list
	while((err = scanf("%d", &next))> 0){
		noInput = 0;
		size ++;
		struct node *temp = malloc(sizeof(struct node));
		if(temp ==NULL){
			fprintf(stderr, "unable to allocted memory\n");
			return 1;
		}
		temp->this = next;
		temp->next = root;
		root = temp;
	}
	sort(root);
	int returnVal = 0;
	if(err == 0 || noInput){
		fprintf(stderr, "err with reading input\n");
		returnVal = 1;
	}
	if(!noInput){
		countMedian(root, size);
	}
	return returnVal;
}

/*
* sort number in order
*/
void sort(struct node* list){
	struct node *ptr1, *ptr2;
	int temp;
	for(ptr1 = list; ptr1 != NULL; ptr1 = ptr1->next){
		for(ptr2 = ptr1->next; ptr2 != NULL; ptr2 = ptr2->next){
			if(ptr1->this > ptr2->this){
				temp = ptr1->this;
				ptr1->this = ptr2->this;
				ptr2->this = temp;
			}
		}
	}
}

/*
* comput median and print the result.
*/
void countMedian(struct node *list, int size){
	float med;
	struct node *curr;
	curr = list; 
	int count = 0, i = 0;
	//if size is even, med is two number in middle divide 2
	if(size%2 == 0){
		while(i<size/2-1){
			curr = curr->next;
			i++;
		}
		count = curr->this + curr->next->this;
		med = (float)count/2;
		
	}else{
		// otherwise, the number in middle.
		while(i<size/2){
			curr = curr->next;
			i++;
		}
		med = curr->this;
	}
	printf("%.1f\n", med);

}
