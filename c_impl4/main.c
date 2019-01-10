#include "data_owner.h"
//#include "my_base64.h"
#include <string.h>
#include <stdio.h>




#include <assert.h>
#include <pbc.h>




void print_hex(char* string);

int main(int argc, char* argv[]){
//int main(){
	if(argc != 2){
		printf("Usage error: %s <param file>\n", argv[0]);
		return -1;
	}
	char* param = argv[1];
	printf("\nParam file is: %s\n\n", param);

	printf("******************************************\nDO1: Submitting file and getting Kf1..\n");
	char* Kf1;// = DO_submit_file("test_file.txt", param); // user 1
	DO_submit_file("test_file.txt", param, Kf1);

	printf("\n******************************************\nDO2: Submitting file and getting Kf2..\n");
	char* Kf2;// = DO_submit_file("test_file.txt", param); // user 2
	DO_submit_file("test_file.txt", param, Kf2);

	printf("\n******************************************\n\n");
	printf("================================\nComparing keys..\n");






/****************************************************/
	// G init
	pairing_t pairing;
	char param_buf[1024];
	FILE* param_fp = fopen(param, "r");
	assert(param_fp != NULL);
	size_t count = fread(param_buf, 1, 1024, /*stdin*/param_fp);
	fclose(param_fp);

	if (!count) pbc_die("input error");
	pairing_init_set_buf(pairing, param_buf, count);

	element_t key1, key2;
	element_init_G1(key1, pairing);
	element_init_G1(key2, pairing);
	element_from_bytes(key1, Kf1);
	element_from_bytes(key2, Kf2);

	element_printf("element1: %B\n", key1);
	element_printf("element2: %B\n", key2);
/****************************************************/













	// Base64 format
/*	size_t* size;
	printf("Kf1: %s\n",Kf1);// base64_encode(Kf1, strlen(Kf1), size));
	printf("Kf2: %s\n\n",Kf2);// base64_encode(Kf2, strlen(Kf2), size));
*/
	// HEX format
	printf("Kf1: ");
	print_hex(Kf1);
	printf("Kf2: ");
	print_hex(Kf2);

	// compare keys
	if(!strcmp(Kf1, Kf2)){//if(!strncmp(Kf1, Kf2, strlen(Kf1))){
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
		printf("%02x", string[i]);
	}
	printf("\n");
}
