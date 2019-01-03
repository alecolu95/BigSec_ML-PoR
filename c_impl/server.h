#include <pbc.h>

typedef struct key_pair {
        element_t first, second;
} KP;
typedef struct server_struct S;

S* S_init(/*g1,g2*/pairing_t pairing);
KP* get_public_key_pair(S* s);
element_t* sign_hash(S* s, element_t hash);
