#include <pbc.h>

int main(){

// pairing init
printf("pairing init\n");
pairing_t pairing;
char param[1024];
size_t count = fread(param, 1, 1024, stdin);
if (!count) pbc_die("input error");
pairing_init_set_buf(pairing, param, count);

printf("\nelems def\n");
element_t g, h;
element_t public_key, secret_key;
element_t sig;
element_t temp1, temp2;

printf("\nelems init\n");
element_init_Zr(secret_key,pairing);
printf("pippo\n");
element_init_G1(h, pairing);
element_init_G1(sig, pairing);
element_init_G2(g, pairing);
element_init_G2(public_key, pairing);
element_init_GT(temp1, pairing);
element_init_GT(temp2, pairing);
//element_init_Zr(secret_key, pairing);

printf("\nelems random\n");
element_random(g);

element_random(secret_key);

printf("\nelem pow zn\n");
element_pow_zn(public_key, g, secret_key);

printf("\nelem from hash\n");
// assuming the hash is "ABCDEF"
element_from_hash(h, "ABCDEF", 6);

element_pow_zn(sig, h, secret_key);

printf("\npairing apply\n");
pairing_apply(temp1, sig, g, pairing);
pairing_apply(temp2, h, public_key, pairing);
if (!element_cmp(temp1, temp2)) {
    printf("signature verifies\n");
} else {
    printf("signature does not verify\n");
}

printf("\nelems clear\n");
element_clear(g);
element_clear(h);
element_clear(public_key);
element_clear(secret_key);
element_clear(sig);
element_clear(temp1);
element_clear(temp2);

return 0;
}
