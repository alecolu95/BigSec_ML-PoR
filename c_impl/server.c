#include "server.h"
#include <math.h>

//in .h: typedef struct key_pair KP;

struct server_struct {
	element_t private_key;
	KP *public_kp;
}; //typedef S;


S* S_init(pairing_t pairing, element_t g1, element_t g2){
	// init
	S* s = (S*) malloc(sizeof(S));
	s->public_kp = (KP*) malloc(sizeof(KP));

	// (g1,g2)
	element_init_G1(s->public_kp->first, pairing);
	element_init_G2(s->public_kp->second, pairing);

	// private key
	element_init_Zr(s->private_key, pairing);
	// assign random value
	element_random(s->private_key);

	element_pow_zn(s->public_kp->first, g1, s->private_key);
	element_pow_zn(s->public_kp->second, g2, s->private_key);

	return s;
}

KP* get_public_key_pair(S* s){
	return s->public_kp;
}

element_t* sign_hash(S* s, element_t hash){
	element_t *signed_h = (element_t*) malloc(sizeof(element_t));
	element_init_same_as(*signed_h, hash);	// copy hash into signed_h
	element_pow_zn(*signed_h, hash, s->private_key);
	return signed_h;
}

void server_free(S* s){
	element_clear(s->private_key);
	element_clear(s->public_kp->first);
	element_clear(s->public_kp->second);
	free(s->public_kp);
}
