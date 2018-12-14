#include <stdio.h>
int main(){
	for(int i = 0; i<20; i++){
		char str1[] = "";
		char str2[] = "0";
		if(i<10){
			printf("./noVowels2 < testcases/test_noVowels_%s%d > my\n",str2, i);
			printf("echo $? >> my\n");
			printf("./exNoVowels2 < testcases/test_noVowels_%s%d > ex\n",str2, i);
			printf("echo $? >> ex\n");
		}else{
			printf("./noVowels2 < testcases/test_noVowels_%s%d > my\n",str1, i);
			printf("echo $? >> my\n");
			printf("./exNoVowels2 < testcases/test_noVowels_%s%d > ex\n",str1, i);
			printf("echo $? >> ex\n");
		}
		printf("diff my ex\n");
	}
}
