#include <stdlib.h>
#ifndef _DEPEND_H_
#define _DEPEND_H_

/*
* File: header.h
* Author: Feiran Yang
* Purpose: header file for depend.c and mymake.c
*/ 

typedef struct node{
    struct node *next;
    char *this;
    int marked;
} node;

typedef struct list{
    node *this;
    struct list *next;
} List;

typedef struct StringList{
    char *this;
    struct StringList *next;
} StringList;

typedef struct tree{
    struct tree *next;
    node *this;
    List *elements;
    StringList *commands;
} tree;


FILE* openFile(char*);
int checkLine(char *string);
void printMallocErr();
void find(tree*, char*);
void traversal(tree*, tree*);
node* addNode(node*, char*);
void addToList(List*, node*);
tree* addTree(tree*, node*);
void addToTree(tree*, node*);
void addCommand(tree*, char*);
void freeString(StringList*);
void freeList(List*);
void freeNode(node*);
void freeTree(tree*);
void freeAndExit();
char* removeSpace(char*);
int checkEmpty(char*);
int checkTarget(char*);

#endif
