
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <gmp.h>
#include <avl.h>

#define P "13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084171"
#define G "11717829880366207009516117596335367088558084999998952205599979459063929499736583746670572176471460312928594829675428279466566527115212748467589894601965568"
#define H "3239475104050450443565264378728065788649097520952449527834792452971981976143292558073856937958553180532878928001494706097394108577585732452307673444020333"
#define B20 1048576

typedef struct part_res_st part_res_t;

struct part_res_st {
	mpz_t val;
	unsigned long x;
	part_res_t * next;
};

/*
void part_res_init(part_res_t * res, const mpz_t val, const unsigned long x) {
	mpz_init_set(res->val, val);
	res->x = x;
}
*/

typedef 
struct l5_lhs_st {
	mpz_t value;
	unsigned long x;
} l5_lhs_t;

l5_lhs_t * l5_lhs_new(mpz_t g, mpz_t h, mpz_t p, unsigned long x) {
	mpz_t tmp;
	l5_lhs_t * ret = (l5_lhs_t*)malloc(sizeof(l5_lhs_t));
	assert(ret != NULL);
	mpz_inits(ret->value, tmp, NULL);

	ret->x = x;

	mpz_powm_ui(tmp, g, x, p);
	mpz_invert(tmp, tmp, p);
	mpz_mul(tmp, h, tmp);
	mpz_mod(ret->value, tmp, p);

	return ret;
}

l5_lhs_t * l5_rhs_new(mpz_t g, mpz_t h, mpz_t p, unsigned long x) {
	mpz_t tmp;
	l5_lhs_t * ret = (l5_lhs_t*)malloc(sizeof(l5_lhs_t));
	assert(ret != NULL);
	mpz_inits(ret->value, tmp, NULL);

	ret->x = x;
	mpz_powm_ui(tmp, g, B20, p);
	mpz_powm_ui(ret->value, tmp, x, p);
	return ret;
}

int l5_cmpr(const void * lhs, const void * rhs) {
	l5_lhs_t * lhs_ptr = (l5_lhs_t*)lhs;
	l5_lhs_t * rhs_ptr = (l5_lhs_t*)rhs;

	return mpz_cmp(lhs_ptr->value, rhs_ptr->value);
}

int main(int argc, char * argv[]) {
	mpz_t p, g, h, b;
	mpz_t tmp, val;
	avl_tree_t * dict = avl_alloc_tree(l5_cmpr, NULL);

	mpz_inits(tmp, val, NULL);
	mpz_init_set_str(p, P, 10);
	mpz_init_set_str(g, G, 10);
	mpz_init_set_str(h, H, 10);
	mpz_init_set_ui(b, B20);


	for(unsigned long int i = 0; i <= B20; i++) {
		l5_lhs_t * r = l5_lhs_new(g, h, p, i);
		//gmp_printf("%Zd\n", r->value);
		avl_node_t * n = avl_insert(dict, r);
		assert(n != NULL);
	}
	for(unsigned long int i = 0; i <= B20; i++) {
		l5_lhs_t * r = l5_rhs_new(g, h, p, i);
		//gmp_printf("%Zd\n", r->value);
		avl_node_t * n = avl_search(dict, r);
		if(n != NULL) {
			l5_lhs_t * f = (l5_lhs_t*)n->item;

			mpz_t result;
			mpz_init(result);
			mpz_set_ui(result, B20);
			mpz_mul_ui(result, result, r->x);
			mpz_add_ui(result, result, f->x);

			gmp_printf("%Zd\n", result);

			exit(0);
		}
	}

	return 0;
}
