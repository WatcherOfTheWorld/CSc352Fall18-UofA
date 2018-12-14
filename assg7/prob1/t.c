#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
	FILE *f;
	printf("%d %s\n",argc, argv[1]);
	if(argv[1] == NULL){
		f = stdin;
	}else{
		f = fopen(argv[1],"r");
	}

	
	char *line = NULL;
	size_t len = 0;
	while(getline(&line, &len, f) != EOF){
		printf("%s",line);
	}
	return 0;

	
}
