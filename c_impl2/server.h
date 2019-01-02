typedef struct server_struct server;

server* S_init(element_t/*TODO generator type*/ g1, element_t/*TODO..*/ g2);
/*key pair type*/ get_public_key_pair(S* s);
char*/*TODO check type*/ sign_hash(S* s, char*/*TODO check type*/ hash);
