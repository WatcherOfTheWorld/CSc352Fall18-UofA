#include <stdio.h>
#include <stdlib.h>
int main(void)
{
int block = 1;
char* line = NULL;
size_t len = 0;
while (getline(&line, &len, stdin) != EOF)
{
/* Because we read as many characters as needed (subject to available
* memory), we will generally have a return in line, and so we do not
* want one in the printf().
*/
printf("Line %03d: %s", block, line);
block++;
}
/*
* When we are done with it, we free line. This will become much more clear
* in a few weeks.
*/
free(line);
return 0;
}
