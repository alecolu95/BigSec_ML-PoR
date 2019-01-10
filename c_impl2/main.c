#include "data_owner.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]){
//int main(){
	if(argc != 2){
		printf("Usage error: %s <param file>\n", argv[0]);
		return -1;
	}
	char* param = argv[1];
	printf("Param file is: %s\n\n", param);

	printf("************************************************************\nSubmitting Kf1..\n");
	char* Kf1 = DO_submit_file("test_file.txt", param); // user 1
	printf("\n************************************************************\nSubmitting Kf2..\n");
	char* Kf2 = DO_submit_file("test_file.txt", param); // user 2

	printf("\n************************************************************\n");
	printf("================================\nComparing keys.. ");
	// compare keys
	if(!strncmp(Kf1, Kf2, strlen(Kf1))){
		printf("Keys are equal!\n");
	} else{
		printf("Keys are different!\n");
	}
	printf("================================\n");
	return 0;
}
