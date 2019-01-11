#ifndef _SERVER_H
#define _SERVER_H

#include <pbc.h>

typedef struct key_pair {
        element_t first, second;
} KP;
typedef struct server_struct S;

S* S_init(char* pairing_file);//pairing_t pairing);
KP* get_public_key_pair(S* s, element_t g1, element_t g2);
element_t* sign_hash(S* s, element_t hash);
void server_free(S* s);

#endif // _SERVER_H
