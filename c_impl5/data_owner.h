#ifndef _DO_H
#define _DO_H

#include "server.h"

void DO_submit_file(char *filename, char* param_file, S* ks, S* cs, unsigned char* res_bytes);//char* param_file);
void print_hex(unsigned char* string);

#endif //_DO_H
