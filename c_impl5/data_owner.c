#include "data_owner.h"
//#include "server.h"
//#include <pbc.h>	//comment if included into server.h
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <assert.h>

#define tau 256 	// security param, hash func
#define random_size 50 	// security param, size of
#define LINE_SIZE 1024

element_t* element_blind(element_t param1, element_t base, element_t exp);
char* my_read_file(char* filename);

// TODO for debug purpose
void print_hex2(char* string);

void DO_submit_file(char *filename, char* param_file, S* ks, S* cs, char* res_bytes){//char* param_file){
	// success or fail flag
	int success = 0;
	unsigned char* res;

	// msg/file to be hashed
//printf("Computing hash (SHA256).. ");
	//char msg[20] = "Hello";
	char *msg = my_read_file(filename);
	unsigned char *hashed_msg = SHA256(msg, strlen(msg), 0);

//printf("Done.\nPairing initialization.. ");
	// G init
	pairing_t pairing;
	char param[1024];

	FILE* param_fp = fopen(param_file, "r");
	assert(param_fp != NULL);

	size_t count = fread(param, 1, 1024, /*stdin*/param_fp);
	fclose(param_fp);
	if (!count) pbc_die("input error");
	pairing_init_set_buf(pairing, param, count);

//printf("Done.\nChoosing random generators g1 and g2.. ");
	element_t gr1, gr2;
	element_init_G1(gr1, pairing);
	element_init_G2(gr2, pairing);
	element_random(gr1);
	element_random(gr2);

//printf("Done.\n\nServers initialization:\n");
	// KS init
//printf("- KS: ");
//	S* ks = S_init(pairing, gr1, gr2);
	// CS init
//printf("Done.\n- CS: ");
//	S* cs = S_init(pairing, gr1, gr2);

//printf("Done.\n\n");


/******************************* LIB SOL. ***********************************/
//printf("Initialization phase.. ");
	element_t /*gr1, gr2, */h;
	element_t public_key, secret_key;
	element_t signed_h;
	element_t temp1, temp2;
// ==================================
	element_t alpha, beta;
// ==================================
	element_t s_signed, s, s_tilde, c_tilde, c;
// ==================================

//	element_init_G2(gr2, pairing);
	element_init_G2(public_key, pairing);
//	element_init_G1(gr1, pairing);
	element_init_G1(h, pairing);
	element_init_G1(signed_h, pairing);
	element_init_GT(temp1, pairing);
	element_init_GT(temp2, pairing);
	element_init_Zr(secret_key, pairing);
// ==================================
	element_init_Zr(alpha, pairing);
	element_init_Zr(beta, pairing);
// ==================================
//printf("elements random\n");
//	element_random(gr1);
//	element_random(gr2);
	element_random(alpha);
	element_random(beta);

//printf("Done.\n=================================================\nSTARTING PROTOCOL..\n");
	// hash msg
//printf("from hash\n");
	element_from_hash(h, hashed_msg, 256); //no. bytes of hashed_msg -> SHA256: 256 fixed length
//printf("Blinding hash with random value alpha.. ");
	// blind signature with alpha
	element_set(signed_h, *element_blind(h, gr1, alpha));

//printf("Done.\n\nKS signature.. ");
	// sign by KS
	// TODO if something doesn't work here, remove s_signed and change it with signed_h
	element_init_same_as(s_signed, signed_h);
	element_set(s_signed, *sign_hash(ks, signed_h));

//printf("Done.\nUnblinding with alpha.. ");
	// unblind to derive signature
	KP *ks_key_pair = get_public_key_pair(ks, gr1, gr2);
//printf("init same\n");
	element_init_same_as(s, s_signed);
//printf("neg\n");
	element_neg(alpha, alpha);		// alpha = -alpha
//printf("element_set with *element_blind()\n");
//	element_init_same_as(s_signed, s);
	element_set(s, *element_blind(s_signed, ks_key_pair->first, alpha));
printf("Verifing KS signature.. ");
	// check signature
	pairing_apply(temp1, s, gr2, pairing);
	pairing_apply(temp2, h, ks_key_pair->second, pairing);
	if(!element_cmp(temp1, temp2)){		// signature verified
printf("OK!\n");//\nBlinding signature with random value beta.. ");
		// blind signature with beta
		element_init_same_as(s_tilde, s);
//		element_random(gr1);		// new random value
		element_set(s_tilde, *element_blind(s, gr1, beta));

//printf("Done.\n\nCS signature.. ");
		// sign by CS
		element_init_same_as(c_tilde, s_tilde);
		element_set(c_tilde, *sign_hash(cs, s_tilde));

//printf("Done.\nUnblinding with beta.. ");
		// unblind to derive signature
		KP *cs_key_pair = get_public_key_pair(cs, gr1, gr2);
        	element_init_same_as(c, c_tilde);
       		element_neg(beta, beta);              // beta = -beta
        	element_set(c, *element_blind(c_tilde, cs_key_pair->first, beta));

printf("Verifing CS signature.. ");
		// check signature
//		element_random(g2);
	        pairing_apply(temp1, c, gr2, pairing);
	        pairing_apply(temp2, s, cs_key_pair->second, pairing);
	        if(!element_cmp(temp1, temp2)){         // signature verified
			printf("OK!\n\n=====================\nSignatures verified!\n=====================\n");

			int size = element_length_in_bytes(c);
			res = (unsigned char*) malloc(size*sizeof(unsigned char*));
			element_to_bytes(res, c);
			success = 1;

		}
		else {
			printf("CS-signature doesn't match!\n");
			success = 0;	// default value
		}

		// free all allocated resources in this context: s_tilde, c_tilde, c
		element_clear(s_tilde);
                element_clear(c_tilde);
                element_clear(c);
	}
	else {
		printf("KS-signature doesn't match!\n");
		success = 0;	// defaut value
	}
//printf("\nClearing memory...");
	//free all allocated resources
	element_clear(gr1);
	element_clear(gr2);
        element_clear(h);
	element_clear(public_key);
	element_clear(secret_key);
	element_clear(signed_h);
	element_clear(temp1);
	element_clear(temp2);
	element_clear(alpha);
	element_clear(beta);
	element_clear(s_signed);
	element_clear(s);

//	server_free(ks);
//	server_free(cs);

//	free(msg);

//	printf("Done.\n\nEnd of the program.\n");

	// return result
	if(success){ // returning Kf
		// TODO debugging
//		printf("--------\nres : ");
//		print_hex2(res);
//		printf("\nhash: ");
	//	printf("--------\nhash: ");
		char* hash_res = SHA256(res, strlen(res), 0);
	//	print_hex2(hash_res);
	//	printf("\n--------\n");

		strncat(res_bytes, hash_res, strlen(hash_res));
	} else {
		res_bytes = NULL;
	}
}

// TODO check pointer return !
element_t* element_blind(element_t param1, element_t base, element_t exp){
	element_t *res = (element_t*) malloc(sizeof(element_t));
	element_init_same_as(*res,param1);
	element_pow_zn(*res, base, exp);
	element_mul(*res, param1, *res);
	return res;
}

int my_get_file_size(FILE* fp){//char* filename){
//	FILE* fp = fopen(filename, "r");
	assert(fp != NULL);
	fseek(fp, 0, SEEK_END);	// seek to end of file
	int res = ftell(fp);	// get current file pointer
	fseek(fp, 0, SEEK_SET);	// seek back to beginning of file
//	fclose(fp);
	return res;
}

char* my_read_file(char* filename){
/*	int file_size = my_get_file_size(filename);
	char* content = (char*) malloc(file_size*sizeof(char));
	char* line = (char*) malloc(LINE_SIZE*sizeof(char));
*/
	FILE* fp = fopen(filename, "r");
	assert(fp != NULL);

	int file_size = my_get_file_size(fp);//filename);
	//printf("File size is: %d\n", file_size);
        char* content = (char*) malloc(file_size*sizeof(char));
        char* line = (char*) malloc(LINE_SIZE*sizeof(char));

	content[0] = '\0';

	while(fgets(line, LINE_SIZE, fp) != NULL){
		//printf("Line length is: %d\n", (int) strlen(line));
		strncat(content, line, strlen(line));//LINE_SIZE);
	}
	fclose(fp);

	free(line);
	//printf("returning file content\n");
	return content;
}

void print_hex2(char* string){
	int i;
	for (i = 0; i < strlen(string); i++) {
		printf("%02x", string[i]);
	}
	printf("\n");
}
