#include "key_server.h"
#include <pbc.h>
#include <math.h>

#define sk_sike 50

struct ks_struct {
	element_t private_key;
	pairing_t/*TODO check type !!!!! */ public_key_pair;
};


KS* KS_init(element_t/*TODO generator type*/ g1, element_t/*TODO..*/ g2){
	KS* ks = (KS*) malloc(sizeof(KS));

	ks->private_key = ; //TODO random bytes
	ks->public_key_pair = ; //TODO (g1,g2)

	return ks;
}

/*key pair type*/ get_public_key_pair(KS* ks){
	return ks->public_key_pair;
}

char* /*TODO check type*/ sign_hash(KS* ks, char*/*TODO check type*/ hash){
	return pow(hash, ks->private_key);
}
