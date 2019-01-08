#include "data_owner.c"
#include <string.h>
#include <stdio.h>

int main(){
	char* Kf1 = DO_submit_file("test_file.txt"); // user 1
	char* Kf2 = DO_submit_file("test_file.txt"); // user 2

	// compare keys
	if(!strncmp(Kf1, Kf2, strlen(Kf1))){
		printf("The keys are equal!\n");
	} else{
		printf("The keys differ!\n");
	}

	return 0;
}
