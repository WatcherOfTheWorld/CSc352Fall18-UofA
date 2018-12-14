#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
file: linked.c
read in command, @addPages to addpage to page linked list
@addLinks to add a link to linked list, add linked pages to link node
@isConnected to check if two pages is connected
*/
int err = 0;

typedef struct node{
	struct node *next;
	char *this;

} node;

typedef struct link{
	int marked;
	node *page2;
	node *page1;
	struct link *next;
} link;

FILE* openFile(char**);
void addPages(node*, char*);
node* findNode(char*, node*);
void addLinks(link*, node*, char*);
void addLink(link*, node*, node*);
void freeNodes(node*);
void freeLinks(link*);
void freeNode(node*);
int findPath(link*, node*, node*);
void resetMarker(link*);

int main(int argc, char *argv[]){
	FILE *file;
	file = openFile(argv);

	char *line = NULL;
	size_t len = 0;
	node *nodehead = NULL;
	// init two nodes and check if malloc fails
	nodehead = malloc(sizeof(node));
	link *linkhead = NULL;
	linkhead = malloc(sizeof(link));
	if(nodehead == NULL || linkhead == NULL){
		fprintf(stderr, "unable to malloc address\n");
		exit(1);
	}

	nodehead->next = NULL;
	linkhead->next = NULL;
	while(getline(&line, &len, file) != EOF){
		int offset = 0;
		char curr[14];
		int retv;
		retv = sscanf(line, "%13s%n", curr, &offset);
		// check command
		if(retv == EOF){
			//do nothing
		}else if(strcmp(curr, "@addPages") == 0){
			addPages(nodehead, line+offset);
		}else if(strcmp(curr, "@addLinks") == 0){
			addLinks(linkhead, nodehead, line+offset);
		}else if(strcmp(curr, "@isConnected") == 0){
			node *start, *des;
			char str[65];
			char *newLine;
			newLine = line+offset;
			// check if there is two pages name has been inputed, if not two name
			// print err msg
			if(sscanf(newLine,"%64s%n", str, &offset)!=EOF){
				start = findNode(str, nodehead);
				newLine+=offset;
				if(sscanf(newLine,"%64s%n", str, &offset)!=EOF){
					newLine+= offset;
					des = findNode(str, nodehead);
					if(sscanf(newLine,"%64s%n",str,&offset)!=EOF){
						fprintf(stderr, "more than two pages has been inputed\n");
						err = 1;
					}else{
						if(start == NULL||des == NULL){
							err = 1;
							fprintf(stderr, "page not exist\n");
						}else{
							int i = findPath(linkhead, start, des);
							printf("%d\n", i);
							// reset marker in link list to 0
							resetMarker(linkhead);
						}
					}
				}else{
					fprintf(stderr, "must input two pages\n");
					err = 1;
				}
			}else{
				fprintf(stderr, "must input two pages\n");
				err = 1;
			}
		}else{
			err = 1;
			fprintf(stderr, "Not a Command\n");
		}
		// free string
		free(line);
		line = NULL;
	}
	fclose(file); // close file
	// free all ram
	free(line);
	freeLinks(linkhead);
	freeNodes(nodehead);

	return err;
}

/*
* add page node(s) to the end of node list
*/
void addPages(node *head, char *string){
	char currString[65];
	int offset;
	// while if there is more names, add them to listr
	while(sscanf(string, "%64s%n", currString, &offset) != EOF){
		string+=offset;
		node *newNode = NULL;
		newNode = malloc(sizeof(node));
		if(newNode == NULL){
			fprintf(stderr, "err in malloc space\n");
			exit(1);
		}
		node *temp;
		temp = head;
		int skip = 0;
		while(temp->next!=NULL){
			temp = temp->next;
			// skipif page arealdy exist
			if(strcmp(temp->this, currString) == 0){
				skip = 1;
				err = 1;
				free(newNode);
				fprintf(stderr, "the page %s is arealdy exist\n", temp->this);
				continue;
			}
		}
		if(skip){
			continue;
		}
		newNode->this = strdup(currString);
		newNode->next = NULL;
		temp->next = newNode;
	}
}

/*
* take link list and node list and string, add pare node in string to link list
*/
void addLinks(link *linkhead, node *nodehead, char *string){
	int offset;
	char currString[65];
	// try to get the first node
	if(sscanf(string, "%64s%n", currString, &offset)!=EOF){
		string+= offset;
		node *sourcePage = findNode(currString, nodehead);
		// if not source page, is an err
		if(sourcePage ==NULL){
			err = 1;
			fprintf(stderr, "the srouce page %s does not exist\n", currString);
			return;
		}
		// try to add all other pages to source page
		while(sscanf(string, "%64s%n", currString, &offset)!= EOF){
			string += offset;
			node *linkPage =findNode(currString, nodehead);
			// if not exist, is an err
			if(linkPage ==NULL){
				fprintf(stderr, "the page %s does not exist\n", currString);
				err = 1;
				continue;
			}
			// add to link pair to link list
			addLink(linkhead, sourcePage, linkPage);
		}
	}
}

/*
* add two pages as pair to link list
*/
void addLink(link *head, node *page1, node *page2){
	if(page1 == page2){
		// skip when page1 == page2
		return;
	}
	while(head->next != NULL){
		head = head->next;

		// skip when link arealdy exsit
		if(head->page1 == page1){
			if(head->page2 == page2){
				return;
			}
		}
	}

	// add a link to the end of link list
	link *newLink = NULL;
	newLink = malloc(sizeof(link));
	if(newLink == NULL){
		fprintf(stderr, "unable to malloc space\n");
		exit(1);
	}
	// set ref
	newLink->page1 = page1;
	newLink->page2 = page2;
	newLink->marked = 0;
	newLink->next = NULL;
	head->next = newLink;
}

/*
* return a node has string page,
* if such node does not exist, return null
*/
node* findNode(char *page, node *head){
	while(head->next!= NULL){
		head = head->next;
		if(strcmp(page, head->this)==0){
			return head;
		}
	}
	return NULL;
}

/*
* find a path from node start to node des
* if result has been found, return 1
* otherwise, return 0;
*/
int findPath(link *root, node *start, node *destination){
	// if node start is node des, return 1
	if(start == destination){
		return 1;
	}

	link *head;
	head = root;
	// find the answer by using dfs
	while(head->next!= NULL){
		head = head->next;
		if(head->marked == 0){
			if(head->page1 == start){
				head->marked = 1;
				// if found the path, return 1
				if(head->page2 == destination){
					return 1;
				}else{
					// if have same start
					int i = findPath(root, head->page2, destination);
					// if found the path, return 1;
					if(i){
						return i;
					}
				}
			}
		}
	}
	return 0;
}

/*
* reset markers in link list to 0;
*/
void resetMarker(link *head){
	while(head->next != NULL){
		head = head->next;
		head->marked = 0;
	}
}

/*
* return a file if an command line argv has been given
* return stdin otherwise
*/
FILE* openFile(char *argv[]){
	if(argv[1] == NULL){
		return stdin;
	}
	FILE *file;
	file = fopen(argv[1], "r");
	if(file == NULL){
		fprintf(stderr, "File not exist\n");
		exit(1);
	}
	return file;
}

/*
* free all links and nodes
*/
void freeLinks(link *head){
	if(head->next!= NULL){
		freeLinks(head->next);
	}
	free(head);
}
void freeNodes(node *head){
	if(head->next!= NULL){
		freeNode(head->next);
	}
	free(head);
}
void freeNode(node *head){
	if(head->next != NULL){
		freeNode(head->next);
	}
	free(head->this);
	free(head);
}