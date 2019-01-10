#include "data_owner.h"
#include <string.h>
#include <stdio.h>

int main(){
printf("Submitting Kf1.. ");
	char* Kf1 = DO_submit_file("test_file.txt"); // user 1
printf("Done.\nSubmittinh Kf2.. ");
	char* Kf2 = DO_submit_file("test_file.txt"); // user 2

printf("Done.\n\nComparing keys.. ");
	// compare keys
	if(!strncmp(Kf1, Kf2, strlen(Kf1))){
		printf("Keys are equal!\n");
	} else{
		printf("Keys are different!\n");
	}

	return 0;
}
