#include "data_owner.h"
#include "my_base64.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

void print_hex(char* string);

int main(int argc, char* argv[]){
//int main(){
	if(argc != 2){
		printf("Usage error: %s <param file>\n", argv[0]);
		return -1;
	}
	//char* param = argv[1];
	//printf("\nParam file is: %s\n\n", param);

	pairing_t pairing;
        char param[1024];

        FILE* param_fp = fopen(argv[1], "r");
        assert(param_fp != NULL);

        size_t count = fread(param, 1, 1024, /*stdin*/param_fp);
        fclose(param_fp);
        if (!count) pbc_die("input error");
        pairing_init_set_str(pairing, param);

	S* ks = S_init(pairing, NULL, NULL);
	S* cs = S_init(pairing, NULL, NULL);

	printf("******************************************\nDO1: Submitting file and getting Kf1..\n");
	printf("Private Keys 1:\n");
	unsigned char* Kf1;
	Kf1 = malloc(33*sizeof(unsigned char));
	printf("\nOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n%c\n", Kf1[0]);
	printSHA256(Kf1);
	DO_submit_file("test_file.txt", param, ks, cs, pairing, Kf1); // user 1
	printf("\n******************************************\nDO2: Submitting file and getting Kf2..\n");
	printf("Private Keys 1:\n");
	unsigned char* Kf2;
	Kf2 = malloc(33*sizeof(unsigned char));
	DO_submit_file("test_file.txt", param, ks, cs, pairing, Kf2); // user 2

	printf("\n******************************************\n\n");
	printf("================================\nComparing keys..\n");

	// Base64 format
	size_t* size;
	//printf("Kf1: %s\n",Kf1);// base64_encode(Kf1, strlen(Kf1), size));
	//printf("Kf2: %s\n\n",Kf2);// base64_encode(Kf2, strlen(Kf2), size));
	printSHA256(Kf1);
	printSHA256(Kf2);

	// HEX format
/*	printf("Kf1: ");
	print_hex(Kf1);
	printf("Kf2: ");
	print_hex(Kf2);
*/
	// compare keys
	if(!strncmp(Kf1, Kf2, strlen(Kf1))){
		printf("Keys are equal!\n");
	} else{
		printf("Keys are different!\n");
	}
	printf("================================\n");
	return 0;
}

void print_hex(char* string){
	int i;
	for (i = 0; i < strlen(string); i++) {
		printf("%02X", string[i]);
	}
	printf("\n");
}
