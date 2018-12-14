#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*
* File: noVowels2.c
* Author: Feiran Yang
* Purpose: read in series of string, find words have same no vowels pattern
* with the other words
* print every set of no vowels pattern words in on line
*/ 

// data struct
typedef struct node{
	char *string;
	struct node *next;
	char *noVowel;
	int checked;
} node;

typedef struct list{
	char* noVowel;
	struct list *next;
	node *word;
} list;

int removeVowels(char[], char[]);
void printResult(list*);

int main(void){
	list *root = malloc(sizeof(list));
	if(root == NULL){
		fprintf(stderr, "err\n");
		return 1;
	}
	root->word = NULL;
	root->noVowel = NULL;
	root->next = NULL;
	char string[65];
	char noVowel[65];
	list *temp = NULL;
	list *next;
	int ret = 0;

	// read from stdin
	while(scanf("%64s", string)!=EOF){
		temp = root;
		if(removeVowels(string, noVowel)){
			// skip when bad input
			fprintf(stderr, "Bad word: %s\n", string);
			ret = 1;
			continue;
		}else if(*noVowel == 0){
			// skip when the noVowel partten is empty
			continue;
		}

		int added = 0;
		// try find the novowel pattern 
		while(temp->next != NULL){
			temp = temp->next;
			if(strcmp(temp->noVowel, noVowel)==0){
				added = 1;
				break;
			}
		}

		// if the no vowel pattern not exsit, create one
		if(!added){
			next = malloc(sizeof(list));
			if(next ==NULL){
				fprintf(stderr, "err\n");
				return 1;
			}
			next->noVowel = strdup(noVowel);
			next->word = malloc(sizeof(node));
			if(next == NULL){
				fprintf(stderr, "err\n");
				return 1;
			}
			next->word->next = NULL;
			temp->next = next;
			temp = temp->next;
			temp->next =NULL;
		}

		// add the input string to this pattern list
		node *head;
		head = temp->word;
		while(head->next!= NULL){
			head = head->next;
		}
		node *new = malloc(sizeof(node));
		if(new ==NULL){
			fprintf(stderr, "err\n");
			return 1;
		}
		new->string = strdup(string);
		new->next =NULL;
		head->next = new;
	}

	printResult(root);

	return ret;
}


/*
* print out the result in each lines
*/
void printResult(list *temp){
	while(temp->next!= NULL){
		temp = temp->next;
		node *head = temp->word;
		// print words have same pattern 
		while(head->next!= NULL){
			head = head->next;
			printf("%s ", head->string);
		}
		printf("\n");
	}
}

/*
* remove all vowels from first array and put the result 
* back into the sec array
* return 0 if no err has occured, return 1 otherwise
*/
int removeVowels(char origal[], char target[]){
	int k = 0;
	int i;
	for(i = 0; i<64; i++){
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
