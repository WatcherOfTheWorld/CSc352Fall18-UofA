#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
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
int workDone = 0;

int main(int argc, char **argv){
    int useDef = 0;
    // check argvs
    if(argc >1 && strcmp("-f", argv[1]) == 0){
        if(argc >= 3){
            if(strcmp("-f", argv[2])== 0){
                fprintf(stderr, "Usage: mymake [-f filename] [target]\n");
                return 1;
            }
        }else if(argc == 4){
            if(strcmp("-f", argv[3])== 0){
                fprintf(stderr, "Usage: mymake [-f filename] [target]\n");
                return 1;
            }
        }else{
            fprintf(stderr, "Usage: mymake [-f filename] [target]\n");
            return 1;
        }
    }else{
        useDef = 1;
        if(argc > 2){
            fprintf(stderr, "Usage: mymake [-f filename] [target]\n");
            return 1;
        }
    }
    if(useDef) {
        file = openFile("myMakefile");
    }else {
        file = openFile(argv[2]);
    }
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
    char *target;
    int useDefTarget = 1;
    // check if need store target for further
    if(useDef && argc == 2){
        target = strdup(argv[1]);
        useDefTarget = 0;
    }else if((!useDef) && argc > 3){
        target = strdup(argv[3]);
        useDefTarget = 0;
    }

    line = NULL;
    size_t len = 0;
    tree *thisTree;
    int first = 1;
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
            // add target and dependence
            const char s[2] = ":";
            char *temp;
            temp = strtok(line, s);

            int count = 0;
            while(temp != NULL){
                char *other;
                other = temp;
                char next[65];
                int offset;
                while(sscanf(other,"%64s%n", next , &offset)!=EOF){
                    other += offset;
                    if(first && useDefTarget){
                        target = strdup(next);
                        first = 0;
                    }
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
                temp = strtok(NULL, s);
            }
        }
    }
    // find result
    find(treeHead, target);
    if(!workDone){
        printf("%s is up to date.\n", target);
    }
    // clear space and exit with err code
    free(target);
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
        fprintf(stderr, "command without target\n");
        free(string);
        freeAndExit();
        exit(1);
    }


    int count = 0;
    char s[2] = ":";
    temp = strtok(string, s);

    char next[65];
    int offset;
    int retv = sscanf(temp, "%64s%n", next, &offset);
    // check if more than one target or no target
    if(retv == EOF){
        fprintf(stderr, "wrong format : no target found\n");
        free(string);
        freeAndExit();
        exit(1);
    }else{
        retv = sscanf(temp+offset, "%64s%n", next, &offset);
        if(retv != EOF){
            fprintf(stderr, "wrong format : more than one or no target at this line 0\n");
            free(string);
            freeAndExit();
            exit(1);
        }
    }

    while(temp != NULL){
        count ++;
        temp = strtok(NULL, s);
    }

    if(count > 2){
        fprintf(stderr, "wrong format : more than one target at this line 1\n");
    }
    timer+=1;
    free(string);
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
    workDone = 1;
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
    // get data for curr target. if target not exist, we need process command anyway
    struct stat targetInfo;
    targetInfo.st_mtim.tv_sec = 0;
    targetInfo.st_mtim.tv_nsec = 0;
    long targetSec = 0, targetNSec = 0;
    if(stat(root->this->this, &targetInfo) != 0) {
        root->this->must = 1;
    }else{
        targetSec = targetInfo.st_mtim.tv_sec;
        targetNSec = targetInfo.st_mtim.tv_nsec;
    }

    while(elements->next != NULL){
        elements = elements->next;
        tree *temp = head;
        int found= 0;
        while(temp->next != NULL){
            temp = temp->next;
            if(temp->this == elements->this){
                traversal(head, temp);
                found = 1;
                // get last modify time for dependence
                struct stat dependenceInfo;
                dependenceInfo.st_mtim.tv_sec=0;
                dependenceInfo.st_mtim.tv_nsec=0;
                long dependenceSec = 0, dependenceNSec =0;
                if(stat(temp->this->this, &dependenceInfo) == 0){
                    dependenceSec = dependenceInfo.st_mtim.tv_sec;
                    dependenceNSec = dependenceInfo.st_mtim.tv_nsec;
                }
                if(temp->this->processed == 0){
                    fprintf(stderr, "cycle found\n");
                }else if(root->this->must != 1){
                    // if last modify time for dependence is newer then target, target must be re-compile
                    if(dependenceSec > targetSec){
                        root->this->must = 1;
                    }else if(dependenceSec == targetSec){
                        if(dependenceNSec > targetNSec){
                            root->this->must = 1;
                        }
                    }
                }
                break;
            }
        }

        if(!found){
            if(elements->this->marked != 1) {
                // if any curr dependence is newer then target,  command need to be process
                //printf("%s\n", elements->this->this);
                struct stat dependenceInfo;
                dependenceInfo.st_mtim.tv_sec=0;
                dependenceInfo.st_mtim.tv_nsec=0;
                long dependenceSec = 0, dependenceNSec=0;
                if(0 != stat(elements->this->this, &dependenceInfo)){
                    fprintf(stderr, "dependence file not find");
                    freeAndExit();
                    exit(1);
                }else{
                    dependenceSec = dependenceInfo.st_mtim.tv_sec;
                    dependenceNSec = dependenceInfo.st_mtim.tv_nsec;
                }
                if (dependenceSec > targetSec) {
                    root->this->must = 1;
                }else if(dependenceSec == targetSec){
                    if(dependenceNSec > targetNSec){
                        root->this->must = 1;
                    }
                }
                elements->this->marked = 1;
            }
        }
    }

    //printf("%s\n", root->this->this);
    StringList *commands = root->commands;
    if(root->this->must == 1) {
        while (commands->next != NULL) {
            // if we need process command, update workDone to 1, and execute command
            commands = commands->next;
            printf("%s", commands->this);
            if(system(commands->this) != 0) {
                fprintf(stderr, "command failed");
                freeAndExit();
                exit(1);
            }
        }
        workDone = 1;
    }
    root->this->processed = 1;
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
