#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
file: linked.c
read in two phone numbers a times from file via argv
read in two phone numbers from stdin and find if they are 
connected
*/

int  err = 0;

typedef struct node{
	struct node *next;
	char *number;
	int queued;
} phone;

typedef struct link{
	struct link *next;
	phone *ANum;
	phone *BNum;
	int count;
} link;

typedef struct queue{
	struct queue *next;
	phone *this;
} queue;
typedef struct array{
	char *string1;
	char *string2;
} array;

int vaildNumber(char*);
phone* addnode(phone*, char*);
FILE* openFile(char*);
int checkLine(char*);
void freeNodes(phone*);
void freeNode(phone*);
void freeLinks(link*);
void freeQueue(queue*);
void addLink(link*, phone*, phone*);
void addToQueue(queue*, phone*);
int find(link*, phone*, queue*, int);
phone* pop(queue*);
int BFS(link*, phone*, phone*);
phone* getNode(phone*,char*);
void resetNodes(phone*);
array* getString(char*);
int findTimes(link*, phone*, phone*);



int main(int argc, char *argv[]){
	// init programe
	int i;
	int count = 0;
	phone *phoneHead;
	link *linkHead;
	phoneHead = malloc(sizeof(phone));
	linkHead = malloc(sizeof(link));
	if(phoneHead == NULL || linkHead == NULL){
		fprintf(stderr, "failed to malloc space\n");
		exit(1);
	}
	phoneHead->number = NULL;
	phoneHead->next = NULL;
	linkHead->next = NULL;
	linkHead->ANum = NULL;
	linkHead->BNum = NULL;

	// read in file
	for(i = 0;i< argc-1; i++){
		FILE* file;
		file = openFile(argv[i+1]);
		if(file == NULL){
			count ++;
			continue;
		}

		char *line = NULL;
		size_t len = 0; 
		// add lines in file to struct 
		while(getline(&line, &len, file)!=EOF){
			array *strings =  getString(line);
			if(strings == NULL){
				continue;
			}
			char *A = strings->string1;
			char *B = strings->string2;

			phone *nodeA = addnode(phoneHead,A);
			phone *nodeB = addnode(phoneHead,B);
			free(A);
			free(B);
			free(strings);
			addLink(linkHead, nodeA, nodeB);
			addLink(linkHead, nodeB, nodeA);
		}
		free(line);
		fclose(file);
	}

	// if no file has been readin
	if(1 == argc){
		fprintf(stderr, "no file can be readin\n");
		free(phoneHead);
		free(linkHead);
		return 1;
	}

	// read inc command from stdin
	char *line = NULL;
	size_t len = 0; 
	while(getline(&line, &len, stdin)!=EOF){
		// printf("%s\n", line);
		array *strings =  getString(line);
		if(strings == NULL){
			continue;
		}
		char *A = strings->string1;
		char *B = strings->string2;
		phone *nodeA = getNode(phoneHead,A);
		phone *nodeB = getNode(phoneHead,B);
		free(strings);

		if(nodeA == NULL || nodeB == NULL){
			fprintf(stderr, "Number not exist\n");
			err  =1;
			free(A);
			free(B);
			continue;
		}

		int i = BFS(linkHead,nodeA, nodeB);
		//print connect deteil 
		if(i == 0){
			int times = findTimes(linkHead, nodeA, nodeB);
			printf("Talked %d times\n", times);
		}else if(i == -1){
			printf("Not connected\n");
		}else{
			printf("Connected through %d numbers\n", i);
		}
		free(A);
		free(B);
		resetNodes(phoneHead);
	}
	// free memory 
	free(line);
	freeNodes(phoneHead);
	freeLinks(linkHead);
	return err;
}

/*
* return two strings contains two numbers,
* if number not vaild, return NULL
*/
array* getString(char *line){
	// check if line is vaild
	int check = checkLine(line);
	if(!check){
		fprintf(stderr, "format err: %s", line);
		err = 1;
		return NULL;
	}
	if(check == -1){
		// return if line is empty
		return NULL;
	}
	char next[13];
	int offSet;
	char *A;
	char *B;
	sscanf(line,"%12s%n",next , &offSet);
	// check if first number is vaild
	if(vaildNumber(next)){
		A = strdup(next);
		sscanf(line+offSet, "%12s%n",next, &offSet);
		// check if first number == 2nd number
		if(strcmp(next, A)== 0){
			free(A);
			err = 1;
			fprintf(stderr, "duplicate item\n");
			return NULL;
			// check if 2nd numbwe is vaild
		}else if(vaildNumber(next)){
			B = strdup(next);
		}else{
			free(A);
			err = 1;
			fprintf(stderr, "not vaild phone number\n");
			return NULL;
		}
	}else{
		err = 1;
		fprintf(stderr, "not vaild phone number\n");
		return NULL;
	}
	array *strings = malloc(sizeof(array));
	if(strings == NULL){
		fprintf(stderr, "Unable to malloc space\n");
		exit(1);
	}
	strings->string1 = A;
	strings->string2 = B;
	return strings;
}

/*
* find how many times this two number has talked
*/
int findTimes(link *head, phone *number1, phone *number2){
	while(head->next != NULL){
		head = head->next;
		if(head->ANum == number1){
			if(head->BNum == number2){
				return head->count;
			}
		}
	}
	// if no found, return 0
	return 0;
}

/*
* check if number is vaild
*/
int vaildNumber(char *number){
	// check if len is 12
	int len = strlen(number);
	if(len != 12){
		return 0;
	}
	int i;
	// check if '-' and number in right position 
	for(i = 0; i< len; i++){
		if(i == 3 || i==7){
			if(number[i] != '-'){
				return 0;
			}
		}else{
			if(number[i]<'0' || number[i]>'9'){
				return 0;
			}
		}
	}
	// correct return 1; otherwise return 0;
	return 1;
}

/*
* return node by number, not exsit return NULL;
*/
phone* getNode(phone *head,char *string){
	while(head->next != NULL){
		head = head->next;
		if(strcmp(head->number, string) == 0){
			return head;
		}
	}
	return NULL;
}

/*
* return a file from file name, unable to open return NULL w/ err msg
*/
FILE* openFile(char argv[]){
	FILE *file;
	file = fopen(argv, "r");
	if(file == NULL){
		fprintf(stderr, "Unable to open this file\n");
		err = 1;
		return NULL;
	}
	return file;
}

/*
* add a node to list, return that node;
* if node already exist, return that node w/o add any thing
*/
phone* addnode(phone *head, char number[]){
	while(head->next != NULL){
		head = head->next;
		if(strcmp(head->number, number) == 0){
			return head;
		}
	}
	// add new node to tail
	phone *newPhone;
	newPhone = malloc(sizeof(phone));
	if(newPhone == NULL){
		fprintf(stderr, "failed to malloc ram\n");
		exit(1);
	}
	newPhone->next = NULL;
	newPhone->number = strdup(number);
	newPhone->queued = 0;
	head->next = newPhone;
	return newPhone;
}

/*
* add link w/ number1& number2 to list
* if link exsit, increment the count by 1
*/
void addLink(link *head, phone *number1, phone *number2){
	while(head->next != NULL){
		head = head->next;
		if(head->ANum == number1){
			if(head->BNum == number2){
				head->count ++;
				return;
			}
		}
	}
	// add new link
	link *newLink = malloc(sizeof(link));
	if(newLink == NULL){
		fprintf(stderr, "Unable to malloc space\n");
		exit(1);
	}

	newLink->ANum = number1;
	newLink->BNum = number2;
	newLink->count = 1;
	newLink->next = NULL;
	head->next = newLink;
}

/*
* return -1 if line is empty; return 0 if the line is in vaild;
* return 1 if line is vaild;
*/
int checkLine(char *line){
	int offSet = 0;
	char curr[13];
	int retv;
	retv = sscanf(line, "%12s%n", curr, &offSet);
	if(retv == EOF){
		//skip line
		return -1;
	}
	line = line+offSet;
	retv = sscanf(line, "%12s%n", curr, &offSet);
	if(retv == EOF){
		return 0;
	}
	retv = sscanf(line+offSet, "%12s%n", curr, &offSet);
	if(retv != EOF){
		return 0;
	}
	return 1;
}

/*
* start a bfs search to find sortest path from start to des
*/
int BFS(link *head, phone *start, phone *des){
	queue *q = malloc(sizeof(queue));
	if(q == NULL){
		fprintf(stderr, "Unable to malloc space\n");
		exit(1);
	}
	q->this = NULL;
	q->next = NULL;
	addToQueue(q, start);
	return find(head, des, q, 0);
}

/*
* return sortest path to des
*/
int find(link *head, phone *des, queue *q, int level){
	//printf("目标:%s\n", des->number);
	// if no more in queue, return -1
	if(q->next == NULL){
		free(q);
		return -1;
	}
	queue *temp = q;
	while(temp->next != NULL){
		temp = temp->next;
		// if item in queue == des, return level-1
		if(strcmp(temp->this->number,des->number) == 0){
			freeQueue(q);
			return level-1;
		}
	}

	queue *newQ = malloc(sizeof(queue));
	if(newQ == NULL){
		fprintf(stderr, "Unable to malloc space\n");
		exit(1);
	}
	newQ->next = NULL;
	newQ->this = NULL;
	// add child of curr queue into new queue at start a new search
	while(q->next != NULL){
		phone *start = pop(q);
		link *tempHead = head;
		while(tempHead->next != NULL){
			tempHead = tempHead->next;
			if(strcmp(tempHead->ANum->number, start->number)==0){
				addToQueue(newQ, tempHead->BNum);
			}
		}
	}
	free(q);
	return find(head, des, newQ, level+1);

}

/*
* add a item to queue, skip item that has aready exsit or marked as queued
*/
void addToQueue(queue *q, phone *node){
	if(node->queued){
		return;
	}
	while(q->next != NULL){
		q = q->next;
		if(q->this == node){
			return;
		}
	}
	queue *newQueue = malloc(sizeof(queue));
	if(newQueue == NULL){
		fprintf(stderr, "failed to malloc space\n");
		exit(1);
	}
	newQueue->next = NULL;
	newQueue->this = node;
	node->queued = 1;
	q->next = newQueue;
}

// return the first item from queue and remove it from queue
phone* pop(queue *q){
	if(q->next!= NULL){
		queue *next = q->next;
		q->next = next->next;
		phone *node = next->this;
		free(next);
		return node;
	}
	return NULL;
}

// reset all node's marker to 0
void resetNodes(phone *head){
	while(head->next != NULL){
		head = head->next;
		head->queued = 0;
	}
}


// free struct
void freeNodes(phone *head){
	if(head->next!= NULL){
		freeNode(head->next);
	}
	free(head);
}
void freeNode(phone *head){
	if(head->next != NULL){
		freeNode(head->next);
	}
	free(head->number);
	free(head);
}
void freeLinks(link *head){
	if(head->next!= NULL){
		freeLinks(head->next);
	}
	free(head);
}
void freeQueue(queue *q){
	if(q->next != NULL){
		freeQueue(q->next);	
	}	
	free(q);
}