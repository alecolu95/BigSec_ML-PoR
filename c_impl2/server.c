#include "server.h"
#include <pbc.h>
#include <math.h>

#define sk_sike 50

struct server_struct {
	element_t private_key;
	pairing_t/*TODO check type !!!!! */ public_key_pair;
};


server* S_init(element_t/*TODO generator type*/ g1, element_t/*TODO..*/ g2){
	S* s = (S*) malloc(sizeof(S));

	s->private_key = ; //TODO random bytes
	s->public_key_pair = ; //TODO (g1,g2)

	return s;
}

/*key pair type*/ get_public_key_pair(S* s){
	return s->public_key_pair;
}

char* /*TODO check type*/ sign_hash(S* s, char*/*TODO check type*/ hash){
	return pow(hash, s->private_key);
}
