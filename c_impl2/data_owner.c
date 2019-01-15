#include "data_owner.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <assert.h>

#define LINE_SIZE 1024

element_t* element_blind(element_t param1, element_t base, element_t exp);
char* my_read_file(char* filename);

void DO_submit_file(char *filename, pairing_t pairing, S* ks, S* cs, unsigned char* res_bytes){
	// result variables
	int success = 0, size;
	unsigned char* res;

	// file to be hashed
	char *msg = my_read_file(filename);
	unsigned char *hashed_msg = SHA256(msg, strlen(msg), 0);

	// generators initialization
	element_t gr1, gr2;
	element_init_G1(gr1, pairing);
	element_init_G2(gr2, pairing);
	element_random(gr1);
	element_random(gr2);

/******************************* LIB SOL. ***********************************/

	element_t h;
	element_t secret_key;
	element_t signed_h;
	element_t temp1, temp2;
// ==================================
	element_t alpha, beta;
// ==================================
	element_t s_signed, s, s_tilde, c_tilde, c;
// ==================================

	element_init_G1(h, pairing);
	element_init_GT(temp1, pairing);
	element_init_GT(temp2, pairing);
	element_init_Zr(secret_key, pairing);
// ==================================
	element_init_Zr(alpha, pairing);
	element_init_Zr(beta, pairing);
// ==================================
	element_random(alpha);
	element_random(beta);

	// hash msg
	element_from_hash(h, hashed_msg, 32);	// no. bytes of hashed_msg: SHA256 always on 256 bits = 32 bytes

	// blind signature with alpha
	element_init_same_as(signed_h, h);
	element_set(signed_h, *element_blind(h, gr1, alpha));

	// sign by KS
	element_init_same_as(s_signed, signed_h);
	element_set(s_signed, *sign_hash(ks, signed_h));

	// unblind to derive signature
	KP *ks_key_pair = get_public_key_pair(ks, gr1, gr2);
	element_init_same_as(s, s_signed);
	element_neg(alpha, alpha);		// alpha = -alpha
	element_set(s, *element_blind(s_signed, ks_key_pair->first, alpha));

	// check signature
	printf("Verifing KS signature.. ");
	pairing_apply(temp1, s, gr2, pairing);
	pairing_apply(temp2, h, ks_key_pair->second, pairing);
	if(!element_cmp(temp1, temp2)){		// signature verified
		printf("OK!\n");
		// blind signature with beta
		element_init_same_as(s_tilde, s);
		element_set(s_tilde, *element_blind(s, gr1, beta));

		// sign by CS
		element_init_same_as(c_tilde, s_tilde);
		element_set(c_tilde, *sign_hash(cs, s_tilde));

		// unblind to derive signature
		KP *cs_key_pair = get_public_key_pair(cs, gr1, gr2);
        	element_init_same_as(c, c_tilde);
       		element_neg(beta, beta);	// beta = -beta
        	element_set(c, *element_blind(c_tilde, cs_key_pair->first, beta));


		// check signature
		printf("Verifing CS signature.. ");
	        pairing_apply(temp1, c, gr2, pairing);
	        pairing_apply(temp2, s, cs_key_pair->second, pairing);
	        if(!element_cmp(temp1, temp2)){         // signature verified
			printf("OK!\n\n=====================\nSignatures verified!\n=====================\n");

			// computing result
			size = element_length_in_bytes(c);
			res = (unsigned char*) malloc(size*sizeof(unsigned char));
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

	//free all allocated resources
	element_clear(gr1);
	element_clear(gr2);
        element_clear(h);
	element_clear(secret_key);
	element_clear(signed_h);
	element_clear(temp1);
	element_clear(temp2);
	element_clear(alpha);
	element_clear(beta);
	element_clear(s_signed);
	element_clear(s);

	free(msg);

	// return result
	if(success){
		unsigned char* hash_res = SHA256(res, size, 0);
		//printf("FINAL KEY (HASHED): ");
		//print_hex(hash_res);

		//res_bytes[0] = '\0';
		//strncat(res_bytes, hash_res, size);
		memcpy(res_bytes, hash_res, 32);
		res_bytes[32] = '\0';
	} else {
		res_bytes = NULL;
	}
}

element_t* element_blind(element_t param1, element_t base, element_t exp){
	element_t *res = (element_t*) malloc(sizeof(element_t));
	element_init_same_as(*res,param1);
	element_pow_zn(*res, base, exp);
	element_mul(*res, param1, *res);
	return res;
}

pairing_t* init_pairing(char* param_file){
        pairing_t *pairing = (pairing_t*) malloc(sizeof(pairing_t));
        char param[1024];

        FILE* param_fp = fopen(param_file, "r");
        assert(param_fp != NULL);

        size_t count = fread(param, 1, 1024, param_fp);
        fclose(param_fp);
        if (!count) pbc_die("input error");
        pairing_init_set_str(*pairing, param);
        return pairing;
}

int my_get_file_size(FILE* fp){
	assert(fp != NULL);
	fseek(fp, 0, SEEK_END);	// seek to end of file
	int res = ftell(fp);	// get current file pointer
	fseek(fp, 0, SEEK_SET);	// seek back to beginning of file
	return res;
}

char* my_read_file(char* filename){
	FILE* fp = fopen(filename, "r");
	assert(fp != NULL);

	int file_size = my_get_file_size(fp);
        char* content = (char*) malloc((file_size+1)*sizeof(char));
        char* line = (char*) malloc(LINE_SIZE*sizeof(char));

	content[0] = '\0';
	while(fgets(line, LINE_SIZE, fp) != NULL){
		strncat(content, line, strlen(line));
	}
	content[file_size] = '\0';
	fclose(fp);

	free(line);
	return content;
}

// function to compare keys
int cmp_keys(unsigned char* k1, unsigned char* k2){
	int diff = 0;

	for(int i = 0; i < 32 && !diff; i++){
		if(k1[i] != k2[i]){
			diff = 1;
		}
	}

	return diff;
}

// function to print SHA256-hashed keys
void print_hex(unsigned char* string){
	int i;
	for (i = 0; i < 32; i++) {
		printf("%02X", string[i]);
	}
	printf("\n");
}
