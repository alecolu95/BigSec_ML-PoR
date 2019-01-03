#include "server.h"
//#include <pbc.h>	//comment if included into server.h
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

#define tau 256 	// security param, hash func
#define random_size 50 	// security param, size of

element_t *element_blind(element_t param1, element_t base, element_t exp);


int main(){
	// msg/file to be hashed
	char msg[20] = "Hello";
	unsigned char *hashed_msg = SHA256(msg, strlen(msg), 0);

	// G init
	pairing_t pairing;
	char param[1024];
	size_t count = fread(param, 1, 1024, stdin);
	if (!count) pbc_die("input error");
	pairing_init_set_buf(pairing, param, count);

	// CS init
	S* cs = S_init(/*g1, g2*/pairing); ///TODO params!
	// KS init
	S* ks = S_init(/*g1, g2*/pairing); ///TODO params!





/*
	// gen key
	/*random bytes* / alpha, beta; //TODO

	// hash file
	/*TODO type* / h = ; //TODO

	// randomize hash for KS
	/*TODO type* / h_randomized = h * pow(gen1(), alpha);

	// KS signing
	/*TODO type* / signed_randomized_h = sign_hash(ks, h_randomized);

	// remove KS randomization
	/*TODO type* / ks_pair = get_public_key_pair(ks); //TODO struct with field "first" and "second"
	/*TODO type* / s = signed_randomized_h * pow(ks_pair.first, alpha);

	// verify with bilinear pairing
	if (/* G.pair(s, gen2()) == G.pair(h, ks_pair.second) * /){ //TODO
		// add new randomization for CS
		/*TODO type* / s_randomized = s * pow(gen1(), beta); //TODO

		// CS signing
		/*TODO type* / s_randomized_signed = sign_hash(cs, s_randomized);

		//remove CS randomization
		/*TODO type* / cs_pair = get_public_key_pair(cs); //TODO struct cs_pair -> first,second
		/*TODO type* / c = s_randomized_signed * pow(cs_pair.first, -beta);

		// verify with bilinear pairing
		// ... TODO missing on python implementation
	}
	else {
		fprintf(stderr,"Signing failed.");
		return -1;
	}
*/


/******************************* LIB SOL. ***********************************/
	element_t gr1, gr2, h;
	element_t public_key, secret_key;
	element_t signed_h;
	element_t temp1, temp2;
// ==================================
	element_t alpha, beta;
// ==================================

	element_init_G2(gr2, pairing);
	element_init_G2(public_key, pairing);
	element_init_G1(gr1, pairing);
	element_init_G1(h, pairing);
	element_init_G1(signed_h, pairing);
	element_init_GT(temp1, pairing);
	element_init_GT(temp2, pairing);
	element_init_Zr(secret_key, pairing);
// ==================================
	element_init_Zr(alpha, pairing);
	element_init_Zr(beta, pairing);
// ==================================

	element_random(gr1);
	element_random(gr2);
	element_random(alpha);
	element_random(beta);

	// hash msg
	element_from_hash(h, hashed_msg, 256); //no. bytes of hashed_msg -> SHA256: 256 fixed length

	// blind signature with alpha
	signed_h = *element_blind(h, gr1, alpha);
	// sign by KS
	element_t s_signed = *sign_hash(ks, signed_h);

	// unblind to derive signature
	KP *ks_key_pair = get_public_key_pair(ks);
	element_t s;
	element_init_same_as(s, s_signed);
	element_neg(alpha, alpha);		// alpha = -alpha
	s = *element_blind(s_signed, ks_key_pair->first, alpha);

	// check signature
	pairing_apply(temp1, s, gr2, pairing);
	pairing_apply(temp2, h, ks_key_pair->second, pairing);
	if(!element_cmp(temp1, temp2)){		// signature verified
		// blind signature with beta
	        element_t s_tilde;
		element_init_same_as(s_tilde, s);
		element_random(gr1);		// new random value
		s_tilde = *element_blind(s, gr1, beta);

		// sign by CS
		element_t c_tilde;
//		element_init_same_as(c,s_tilde);
		c_tilde = *sign_hash(cs, s_tilde);

		// unblind to derive signature
		KP *cs_key_pair = get_public_key_pair(cs);
	        element_t c;
        	element_init_same_as(c, c_tilde);
       		element_neg(beta, beta);              // beta = -beta
        	c = *element_blind(c_tilde, cs_key_pair->first, beta);

		// check signature
//		element_random(g2);
	        pairing_apply(temp1, s, gr2, pairing);
	        pairing_apply(temp2, h, cs_key_pair->second, pairing);
	        if(!element_cmp(temp1, temp2)){         // signature verified
			printf("Signatures verified!");
		}
		else {
			printf("CS-signature doesn't match!");
		}

		// free all allocated resources in this context
		// ...
	}
	else {
		printf("KS-signature doesn't match!");
	}

	//free all allocated resources
	element_clear(gr1);
	//.....

	return 0;
}

// TODO check pointer return !
element_t *element_blind(element_t param1, element_t base, element_t exp){
	element_t *res = (element_t*) malloc(sizeof(element_t));
	element_init_same_as(*res,param1);
	element_pow_zn(*res, base, exp);
	element_mul(*res, param1, *res);
	return res;
}
