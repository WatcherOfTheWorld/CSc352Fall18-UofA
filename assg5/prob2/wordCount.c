#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*
* Feiran Yang
* wordCount.c
* count how many times a words occur in input
*/

struct node{
	struct node *next;
	char *string;
	int count;
};

char* strip(char*);
struct node* addCount(struct node*, char*);
void sort(struct node*);
void toLower(char*);

int main(void){
	struct node *root =NULL;
	char string[129];
	while(scanf("%128s",string)!= EOF){
		char *newString = strip(string);
		// if striped string is empty, do nothing
		if(*newString == '\0'){
			continue;
		}
		toLower(newString);
		//printf("err here\n");
		root = addCount(root, newString);
	}

	sort(root);
	while(root != NULL){
		printf("%s %d\n", root->string,root->count);
		root = root->next;
	}


}

/*
* move none-alpha in the end and front
*/
char* strip(char *string){
	int i;
	// remove none-aplpha in end
	for(i = strlen(string)-1; i>=0; i--){
		char currChar = tolower(string[i]);
		if(currChar>'z'|| currChar<'a'){
			// set all to null
			string[i] = 0;
		}else{
			break;
		}
	}
	char *charPtr;

	// return the original ptr if string only contains none-alaph char
	if(string[0] == 0){
		return string;
	}
	// move ptr to the first element that is aplpha
	for(i = 0; i<strlen(string); i++){
		char currChar = tolower(string[i]);
		if(currChar<'a'|| currChar>'z'){
			//do noting
		}else{
			charPtr = string+i;
			break;
		}
	}
	return charPtr;
}

// covert str to lower case
void toLower(char *string){
	int i;
	for(i = 0; i<strlen(string); i++){
		string[i] = tolower(string[i]);
	}
}

// increment count for every words occurs in input
struct node* addCount(struct node *root, char *newString){
	struct node *curr;
	curr = root;
	while(curr != NULL){
		if(curr->string != NULL &&strcmp(newString, curr->string)==0){
			(curr->count)++;
			// return ptr for original root
			return root;
		}else{
			if(curr->next!=NULL){
				curr = curr->next;
			}else{
				break;
			}
		}
	}
	
	// add new node in the front if the word is not in list
	struct node *temp = malloc(sizeof(struct node));
	if(temp ==NULL){
		fprintf(stderr, "unable to allocte memory\n");
		exit(1);
	}
	temp->string = strdup(newString);
	temp->count = 1;
	temp->next = root;
	return temp;

}

/*
* sort the list in alphabet order
*/
void sort(struct node* list){
	struct node *ptr1, *ptr2;
	int temp;
	char *tempString;
	for(ptr1 = list; ptr1 != NULL; ptr1 = ptr1->next){
		for(ptr2 = ptr1->next; ptr2 != NULL; ptr2 = ptr2->next){
			if(strcmp(ptr1->string, ptr2->string)> 0){
				// swich elements
				temp = ptr1->count;
				ptr1->count = ptr2->count;
				ptr2->count = temp;
				tempString = ptr1->string;
				ptr1->string = ptr2->string;
				ptr2->string = tempString;
			}
		}
	}
}