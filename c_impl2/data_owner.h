#ifndef _DO_H
#define _DO_H

#include "server.h"

void DO_submit_file(char *filename, pairing_t pairing, S* ks, S* cs, unsigned char* res_bytes);
pairing_t* init_pairing(char* param_file);
int cmp_keys(unsigned char* k1, unsigned char* k2);
void print_hex(unsigned char* string);

#endif //_DO_H
