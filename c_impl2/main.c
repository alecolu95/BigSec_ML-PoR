#include "data_owner.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("Usage error: %s <param file>\n", argv[0]);
		return -1;
	}
	char* param = argv[1];
	printf("\nParam file is: %s\n\n", param);

	// pairing
	pairing_t *pairing = init_pairing(param);

	// servers
	S* ks = S_init(*pairing);
	S* cs = S_init(*pairing);

	// user 1
	printf("******************************************\nDO1: Submitting file and getting Kf1..\n");
	unsigned char* Kf1 = (unsigned char*) malloc(1024*sizeof(unsigned char));
	DO_submit_file("test_file.txt", *pairing, ks, cs, Kf1);

	// user 2
	printf("\n******************************************\nDO2: Submitting file and getting Kf2..\n");
	unsigned char* Kf2 = (unsigned char*) malloc(1024*sizeof(unsigned char));
	DO_submit_file("test_file.txt", *pairing, ks, cs, Kf2);

	printf("\n******************************************\n\n");
	printf("================================\nComparing keys..\n");
	printf("Kf1: ");
	print_hex(Kf1);
	printf("Kf2: ");
	print_hex(Kf2);

	// compare keys
	if(!cmp_keys(Kf1, Kf2)){ //if(!strcmp(Kf1, Kf2)){
		printf("Keys are equal!\n");
	} else{
		printf("Keys are different!\n");
	}
	printf("================================\n");


	// free resources
	server_free(ks);
	server_free(cs);

	return 0;
}
