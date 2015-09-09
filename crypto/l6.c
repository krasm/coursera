
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include <gmp.h>

#define NT1 "17976931348623159077293051907890247336179769789423065727343008115 \
    77326758055056206869853794492129829595855013875371640157101398586 \
    47833778606925583497541085196591615128057575940752635007475935288 \
    71082364994994077189561705436114947486504671101510156394068052754 \
    0071584560878577663743040086340742855278549092581"

#define RT1 "134078079299425970995740249982058461274793658205923933777\
		23561443721764030073662768891111614362326998675040546094339320\
		838419523375986027530441562135724301"

#define NT2 "6484558428080716696628242653467722787263437207069762630604390703787 \
    9730861808111646271401527606141756919558732184025452065542490671989 \
    2428844841839353281972988531310511738648965962582821502504990264452 \
    1008852816733037111422964210278402893076574586452336833570778346897 \
    15838646088239640236866252211790085787877"

#define RT2 "25464796146996183438008816563973942229341454268524157846328581927885\
	77796998522283514385107324957345410738446155719317330449724481407150579056659\
	3206419759"

#define N1 "17976931348623159077293051907890247336179769789423065727343008115\
    77326758055056206869853794492129829595855013875371640157101398586\
    47833778606925583497541085196591615128057575940752635007475935288\
    71082364994994077189561705436114947486504671101510156394068052754\
    0071584560878577663743040086340742855278549092581"

static void ex1(mpz_t p, mpz_t q, mpz_t N) {
	mpz_t A, t, x;

	mpz_inits (A, t, x, NULL);

	mpz_sqrt (A, N);
	mpz_add_ui(A, A, 1);
	mpz_pow_ui(t, A, 2);
	mpz_sub(x, t, N);
	mpz_sqrt(t, x);
	mpz_sub(p, A, t);
	mpz_add(q, A, t);
}

static void ex2(mpz_t p, mpz_t q, mpz_t N) {
	mpz_t A, t, x, l;
	int i;

	mpz_inits (A, t, x, l, NULL);

	mpz_sqrt (A, N);
	mpz_add_ui(A, A, 1);

	mpz_ui_pow_ui(l, 2, 20);
	for(i = 0; mpz_cmp_ui(l, i) >= 0; i++) {
		mpz_add_ui(t, A, i);
		mpz_pow_ui(x, t, 2);
		mpz_sub(t, x, N);
		mpz_sqrt(x, t);
		/* increment t */
		mpz_sub(p, A, x);
		mpz_add(q, A, x);
#if 0 
		fprintf(stderr, "Testing ");
		mpz_out_str(stderr, 10, p);
		fprintf(stderr, "\n");
#endif
		/* check correctnes */
		mpz_mul(x, p, q);
		if(mpz_cmp(N, x) == 0) {
			mpz_out_str(stderr, 10, p);
			break;
		}
	} 

}

static void test1() {
	mpz_t p, q, N, R;

	mpz_inits(p, q, N, R, NULL);
	mpz_set_str(N, NT1, 10);
	mpz_set_str(R, RT1, 10);

	ex1(p, q, N);
	assert(mpz_cmp(R,p) == 0);
}

static void test2() {
	mpz_t p, q, N, R;

	mpz_inits(p, q, N, R, NULL);
	mpz_set_str(N, NT2, 10);
	mpz_set_str(R, RT2, 10);

	ex2(p, q, N);
	assert(mpz_cmp(R,p) == 0);
}


int main(int argc, char * argv[]) {
	mpz_t p, q, N;

	mpz_inits(p, q, N, NULL);
	mpz_set_str(N, N1, 10);

	test1() ;
	test2();

	return EXIT_SUCCESS;
}
