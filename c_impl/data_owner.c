#include "server.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <assert.h>

#define LINE_SIZE 1024

element_t *element_blind(element_t param1, element_t base, element_t exp);
char* my_read_file(char* filename);


int main(int argc, char* argv[]){
	 if(argc != 2){
                printf("Usage error: %s <param file>\n", argv[0]);
                return -1;
        }
        printf("\nParam file is: %s\n\n", argv[1]);

	// msg/file to be hashed
	printf("Computing hash (SHA256).. ");
	char *msg = my_read_file("test_file.txt");
	unsigned char *hashed_msg = SHA256(msg, strlen(msg), 0);

	printf("Done.\nPairing initialization.. ");
	// G init
	pairing_t pairing;
	char param[1024];
	FILE* param_fp = fopen(argv[1], "r");
	assert(param_fp != NULL);
	size_t count = fread(param, 1, 1024, param_fp);
	fclose(param_fp);
	if (!count) pbc_die("input error");
	pairing_init_set_buf(pairing, param, count);

	printf("Done.\nChoosing random generators g1 and g2.. ");
	element_t gr1, gr2;
	element_init_G1(gr1, pairing);
	element_init_G2(gr2, pairing);
	element_random(gr1);
	element_random(gr2);

	printf("Done.\n\nServers initialization:\n");
	// KS init
	printf("- KS: ");
	S* ks = S_init(pairing, gr1, gr2);
	// CS init
	printf("Done.\n- CS: ");
	S* cs = S_init(pairing, gr1, gr2);

printf("Done.\n\n");


/******************************* LIB SOL. ***********************************/
printf("Initialization phase.. ");
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
	element_init_G1(signed_h, pairing);
	element_init_GT(temp1, pairing);
	element_init_GT(temp2, pairing);
	element_init_Zr(secret_key, pairing);
// ==================================
	element_init_Zr(alpha, pairing);
	element_init_Zr(beta, pairing);
// ==================================
	element_random(alpha);
	element_random(beta);

printf("Done.\n=================================================\nSTARTING PROTOCOL..\n");
	// hash msg
	element_from_hash(h, hashed_msg, 32); //no. bytes of hashed_msg -> SHA256: 256 bits fixed length = 32 bytes
printf("Blinding hash with random value alpha.. ");

	// blind signature with alpha
	element_set(signed_h, *element_blind(h, gr1, alpha));

	printf("Done.\n\nKS signature.. ");
	// sign by KS
	element_init_same_as(s_signed, signed_h);
	element_set(s_signed, *sign_hash(ks, signed_h));

	printf("Done.\nUnblinding with alpha.. ");
	// unblind to derive signature
	KP *ks_key_pair = get_public_key_pair(ks);
	element_init_same_as(s, s_signed);
	element_neg(alpha, alpha);		// alpha = -alpha
	element_set(s, *element_blind(s_signed, ks_key_pair->first, alpha));

	printf("Done.\n\nVerificating signature.. ");
	// check signature
	pairing_apply(temp1, s, gr2, pairing);
	pairing_apply(temp2, h, ks_key_pair->second, pairing);
	if(!element_cmp(temp1, temp2)){		// signature verified
		printf("OK!\n\nBlinding signature with random value beta.. ");

		// blind signature with beta
		element_init_same_as(s_tilde, s);
		element_set(s_tilde, *element_blind(s, gr1, beta));

		printf("Done.\n\nCS signature.. ");
		// sign by CS
		element_init_same_as(c_tilde, s_tilde);
		element_set(c_tilde, *sign_hash(cs, s_tilde));

		printf("Done.\nUnblinding with beta.. ");
		// unblind to derive signature
		KP *cs_key_pair = get_public_key_pair(cs);
        	element_init_same_as(c, c_tilde);
       		element_neg(beta, beta);              // beta = -beta
        	element_set(c, *element_blind(c_tilde, cs_key_pair->first, beta));

		printf("Done.\n\nVerificating signature.. ");
		// check signature
	        pairing_apply(temp1, c, gr2, pairing);
	        pairing_apply(temp2, s, cs_key_pair->second, pairing);
	        if(!element_cmp(temp1, temp2)){         // signature verified
			printf("OK!\n\n=====================\nSignatures verified!\n=====================\n");
		}
		else {
			printf("CS-signature doesn't match!\n");
		}

		// free all allocated resources in this context: s_tilde, c_tilde, c
		element_clear(s_tilde);
                element_clear(c_tilde);
                element_clear(c);
	}
	else {
		printf("KS-signature doesn't match!\n");
	}
printf("\nClearing memory...");
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

	server_free(ks);
	server_free(cs);

printf("Done.\n\nEnd of the program.\n");
	return 0;
}

element_t *element_blind(element_t param1, element_t base, element_t exp){
	element_t *res = (element_t*) malloc(sizeof(element_t));
	element_init_same_as(*res,param1);
	element_pow_zn(*res, base, exp);
	element_mul(*res, param1, *res);
	return res;
}

int my_get_file_size(FILE* fp){
        assert(fp != NULL);
        fseek(fp, 0, SEEK_END); // seek to end of file
        int res = ftell(fp);    // get current file pointer
        fseek(fp, 0, SEEK_SET); // seek back to beginning of file
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
