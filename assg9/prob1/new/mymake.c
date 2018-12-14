#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
/*
* File: mymake.c
* Author: Feiran Yang
* Purpose: print out dependency and command for a makefile and target
*/ 
int retva = 0;
int timer = 0;
node *nodelist;
tree *treeHead;
FILE *file;
char *line;

int main(int argc, char **argv){
    if(argc < 3){
        fprintf(stderr, "Usage: mymake [filename] [target]\n");
        exit(1);
    }

    file = openFile(argv[1]);
    // init program
    nodelist = malloc(sizeof(node));
    treeHead = malloc(sizeof(tree));
    List *l = malloc(sizeof(List));
    StringList *sl = malloc(sizeof(StringList));
    if(nodelist == NULL || treeHead == NULL || l == NULL || sl == NULL){
        printMallocErr();
    }
    l->next = NULL;
    l->this = NULL;
    sl->next = NULL;
    sl->this = NULL;
    nodelist->this = NULL;
    nodelist->next = NULL;
    treeHead->next = NULL;
    treeHead->this = NULL;
    treeHead->commands = sl;
    treeHead->elements = l;

    line = NULL;
    size_t len = 0;
    tree *thisTree;
    while(getline(&line, &len, file)!=EOF){
        // check if line is empty
        if(checkEmpty(line)==1){
            continue;
        }
        // add command to tree
        int rev = checkLine(strdup(line));
        if(rev == -1){
            char *noSpace = removeSpace(line);
            if(noSpace != NULL) {
                addCommand(thisTree, noSpace);
                free(noSpace);
            }

        }else{
            StringList *strings = splitString(line,':');
            StringList *temp = strings;
            int count = 0;
            while(temp->next != NULL){
                temp = temp->next;
                char *other = temp->this;
                char next[65];
                int offset;
                while(sscanf(other,"%64s%n", next , &offset)!=EOF){
                    other += offset;
                    node *item = addNode(nodelist, next);
                    // add dependence
                    if(count == 0){
                        thisTree = addTree(treeHead, item);
                        count ++;
                    // add node
                    }else{
                        addToTree(thisTree, item);
                    }
                }
            }
            freeString(strings);
        }
    }
    // find result
    find(treeHead, argv[2]);
    // clear space and exit with err code
    fclose(file);
    free(line);
    freeNode(nodelist);
    freeTree(treeHead);
    return retva;
}

/*
 * open a file, if failed to, perror();
 */
FILE* openFile(char fileName[]){
    FILE* file = fopen(fileName,"r");
    if(file == NULL){
        perror(fileName);
        exit(1);
    }
    return file;
}

/*
 * check if input line is legit
 */
int checkLine(char *string){
    char *temp = string;

    // if is command
    if(*temp == '\t'){
        free(string);
        if(timer == 0){
            fprintf(stderr, "command without target\n");
            freeAndExit();
            exit(1);
        }
        return -1;
    }

    // if is target:dependence
    if(!checkTarget(string)){
        // check if target have only one ':'
        fprintf(stderr, "command without target or more than one target\n");
        free(string);
        freeAndExit();
        exit(1);
    }

    StringList *strings = splitString(string, ':');

    char next[65];
    int offset;
    int retv = sscanf(strings->next->this, "%64s%n", next, &offset);
    // check if more than one target or no target
    if(retv == EOF){
        fprintf(stderr, "wrong format : no target found\n");
        free(string);
        freeString(strings);
        freeAndExit();
        exit(1);
    }else{
        retv = sscanf(strings->next->this+offset, "%64s%n", next, &offset);
        if(retv != EOF){
            fprintf(stderr, "wrong format : more than one or no target at this line 0\n");
            free(string);
            freeString(strings);
            freeAndExit();
            exit(1);
        }
    }

    timer+=1;
    free(string);
    freeString(strings);
    return 1;
}

/*
 * find make command and dependence
 */
void find(tree *head, char *target){
    tree *temp = head;
    while(temp->next != NULL){
        temp = temp->next;
        if(strcmp(temp->this->this, target) == 0){
            return traversal(head, temp);
        }
    }

    fprintf(stderr, "target does not exist\n");
    retva = 1;
}

/*
 * traversal the tree to find dependence and command that should be printed
 */
void traversal(tree *head, tree *root){
    // if this node has been visited, skip
    if(root->this->marked == 1){
        return;
    }
    root->this->marked = 1;
    List *elements = root->elements;
    while(elements->next != NULL){
        elements = elements->next;
        tree *temp = head;
        int found= 0;
        while(temp->next != NULL){
            temp = temp->next;
            if(temp->this == elements->this){
                traversal(head, temp);
                found = 1;
                break;
            }
        }
        if(!found){
            if(elements->this->marked != 1) {
                printf("%s\n", elements->this->this);
                elements->this->marked = 1;
            }
        }
    }
    printf("%s\n", root->this->this);
    StringList *commands = root->commands;
    while(commands->next != NULL){
        commands = commands->next;
        printf("  %s", commands->this);
    }
}

/*
 * add a tree to treelist;
 * if the tree already exist, print err msg and exit with code 1
 */
tree* addTree(tree *head, node *root){
    // find if duplicate
    while(head->next!= NULL){
        head = head->next;
        if(head->this == root){
            fprintf(stderr, "repeat target");
            freeAndExit();
            exit(1);
        }
    }

    tree *newTree = malloc(sizeof(tree));
    if(newTree == NULL){
        printMallocErr();
    }

    List *elements = malloc(sizeof(List));
    if(elements == NULL){
        printMallocErr();
    }

    elements->next = NULL;
    elements->this = NULL;

    newTree->elements = elements;
    newTree->this = root;
    newTree->next = NULL;
    head->next = newTree;

    // set list to store commands.
    StringList *commands = malloc(sizeof(StringList));
    if(commands == NULL){
        printMallocErr();
    }
    commands->this = NULL;
    commands->next = NULL;
    newTree->commands = commands;
    return newTree;
}

/*
 * free all allocated space
 */
void freeAndExit(){
    fclose(file);
    free(line);
    freeNode(nodelist);
    freeTree(treeHead);
}
