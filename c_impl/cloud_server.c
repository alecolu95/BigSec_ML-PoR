#include "cloud_server.h"
#include <pbc.h>
#include <math.h>

#define sk_sike 50

struct cs_struct {
	element_t private_key;
	pairing_t/*TODO check type !!!!! */ public_key_pair;
};


CS* CS_init(element_t/*TODO generator type*/ g1, element_t/*TODO..*/ g2){
	CS* cs = (CS*) malloc(sizeof(CS));

	cs->private_key = ; //TODO random bytes
	cs->public_key_pair = ; //TODO (g1,g2)

	return cs;
}

/*key pair type*/ get_public_key_pair(CS* cs){
	return cs->public_key_pair;
}

char* /*TODO check type*/ sign_hash(CS* cs, char*/*TODO check type*/ hash){
	return pow(hash, cs->private_key);
}
