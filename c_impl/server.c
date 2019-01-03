#include "server.h"
//#include <pbc.h>	//comment if already included into server.h
#include <math.h>

#define sk_sike 50

//in .h: typedef struct key_pair KP;

struct server_struct {
	element_t private_key;
	KP *public_kp;
}; //typedef S;


S* S_init(/*TODO g1,g2 ?*/pairing_t pairing){
	// init
	S* s = (S*) malloc(sizeof(S));
	s->public_kp = (KP*) malloc(sizeof(KP));

	// (g1,g2)
	element_init_G1(s->public_kp->first, pairing);
	element_init_G2(s->public_kp->second, pairing);
	// private key
	element_init_Zr(s->private_key, pairing);

	// assign random values
	element_random(s->public_kp->first);
	element_random(s->public_kp->second);
	element_random(s->private_key);

	return s;
}

KP* get_public_key_pair(S* s){
	return s->public_kp;
}

// TODO check pointer return !
element_t* sign_hash(S* s, element_t hash){
	element_t *signed_h = (element_t*) malloc(sizeof(element_t));
	element_init_same_as(*signed_h, hash);	// copy hash into signed_h
	element_pow_zn(*signed_h, hash, s->private_key);
	return signed_h;
}
