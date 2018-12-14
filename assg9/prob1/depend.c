#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "header.h"
/*
* File: depend.c
* Author: Feiran Yang
* Purpose: support code for mymake.c. 
* include functions for handling struct and free memory
*/ 

/*
 * add a node to node linked list
 */
node* addNode(node *root, char *string){
    while(root->next != NULL){
        root = root->next;
        if(strcmp(root->this, string) == 0){
            // if node exist, return the node;
            return root;
        }
    }
    node *newNode = malloc(sizeof(node));
    if(newNode == NULL){
        printMallocErr();
    }
    newNode->next = NULL;
    newNode->this = strdup(string);
    newNode->marked = 0;
    root->next = newNode;
    return newNode;
}

/*
 * add a node to a list
 */
void addToList(List *head, node *element){
    while(head->next != NULL){
        head = head->next;
        // skip if element exist
        if(strcmp(head->this->this, element->this) == 0){
            return;
        }
    }

    List *newNode = malloc(sizeof(List));
    if(newNode == NULL){
        printMallocErr();
    }

    newNode->next = NULL;
    newNode->this = element;
    head->next = newNode;
}

/*
 * print malloc failed msg and exist with code 1
 */
void printMallocErr(){
    fprintf(stderr, "Failed to malloc space\n");
    exit(1);
}


/*
 * add a node to as child to tree
 */
void addToTree(tree *root, node *item){
    // if node exist, skip
    if(root->this == item){
        return;
    }
    // otherwise add to the end
    List *itemList = root->elements;
    addToList(itemList, item);
}

/*
 * add command into the root of some tree
 */
void addCommand(tree *root, char *command){
    StringList *list = root->commands;
    // literate to the end of list
    while(list->next != NULL){
        list = list->next;
    }

    // append command to the end of list
    StringList *newList = malloc(sizeof(StringList));
    if(newList == NULL){
        printMallocErr();
    }

    newList->next = NULL;
    newList->this = strdup(command);
    list->next = newList;
}

/*
 * free list of string
 */
void freeString(StringList *head){
    if(head->next != NULL){
        freeString(head->next);
    }
    free(head->this);
    free(head);
}

/*
 * free list of nodes
 */
void freeList(List *head){
    if(head->next != NULL){
        freeList(head->next);
    }
    free(head);
}

/*
 * free all nodes
 */
void freeNode(node *head){
    if(head->next != NULL){
        freeNode(head->next);
    }
    free(head->this);
    free(head);
}

/*
 * free a tree from head
 */
void freeTree(tree *head) {
    if (head->next != NULL) {
        freeTree(head->next);
    }
    freeString(head->commands);
    freeList(head->elements);
    free(head);
}

/*
 * remove any space char in the front
 */
char* removeSpace(char* command){
    char* temp = command;
    while(isspace(*temp)){
        temp += 1;
        if(temp == NULL){
            break;
        }
    }
    char *new = strdup(temp);
    return new;
}

/*
 * check if a string is empty string
 */
int checkEmpty(char* string){
    while(*string != '\n'){
        if(isspace(*string)){
            string+=1;
        }
        else{
            return 0;
        }
    }
    return 1;
}

/*
 * check if there is only one target
 */
int checkTarget(char* string){
    int count = 0;
    while(*string != '\n'){
        if(*string == ':'){
            count ++;
        }
        string+=1;
    }
    if(count == 1){
        return 1;
    }else{
        return 0;
    }
}