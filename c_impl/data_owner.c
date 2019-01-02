#include "cloud_server.h"
#include "key_server.h"

#include <pbc.h>
#include <math.h>
#include <stdio.h>

#define tau 256 	// security param, hash func
#define random_size 50 	// security param, size of

int main(){
	// G init
	// ...
	// CS init
	CS* cs = CS_init(g1, g2); ///TODO params!
	// KS init
	KS* ks = KS_init(g1, g2); ///TODO params!


	// gen key
	/*random bytes*/ alpha, beta; //TODO

	// hash file
	/*TODO type*/ h = ; //TODO

	// randomize hash for KS
	/*TODO type*/ h_randomized = h * pow(gen1(), alpha);

	// KS signing
	/*TODO type*/ signed_randomized_h = sign_hash(ks, h_randomized);

	// remove KS randomization
	/*TODO type*/ ks_pair = get_public_key_pair(ks); //TODO struct with field "first" and "second"
	/*TODO type*/ s = signed_randomized_h * pow(ks_pair.first, alpha);

	// verify with bilinear pairing
	if (/* G.pair(s, gen2()) == G.pair(h, ks_pair.second) */){ //TODO
		// add new randomization for CS
		/*TODO type*/ s_randomized = s * pow(gen1(), beta); //TODO

		// CS signing
		/*TODO type*/ s_randomized_signed = sign_hash(cs, s_randomized);

		//remove CS randomization
		/*TODO type*/ cs_pair = get_public_key_pair(cs); //TODO struct cs_pair -> first,second
		/*TODO type*/ c = s_randomized_signed * pow(cs_pair.first, -beta);

		// verify with bilinear pairing
		// ... TODO missing on python implementation
	}
	else {
		fprintf(stderr,"Signing failed.");
		return -1;
	}

	return 0;
}
