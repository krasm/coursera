#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include <assert.h>
#include <gmp.h>
#include <gmpxx.h>

using namespace std;

inline unsigned char char2hex(char c)
{
   if (c >= '0' && c <= '9') return c - '0';
   if (c >= 'a' && c <= 'f') return c - 'a' + 10;
   if (c >= 'A' && c <= 'F') return c - 'A' + 10;
   throw std::runtime_error("wrong char");
}

unsigned char from_hex(unsigned char msb, unsigned char lsb) {
    unsigned char ret = char2hex(msb);
    ret <<= 4;
    ret |= char2hex(lsb);
    
    return ret;
}

mpz_class ex1(const mpz_class & N) {
    mpz_class A = sqrt(N);
    if(!mpz_perfect_square_p(N.get_mpz_t())) {
        A += 1;
    }

    mpz_class x = sqrt(A*A - N);
    return A - x;
}


mpz_class ex2(const mpz_class & N) {

    mpz_class A = sqrt(N);
    if(!mpz_perfect_square_p(N.get_mpz_t())) {
        A += 1;
    }

    while(A < N) {
        mpz_class x = sqrt(A*A - N);
        mpz_class p = A -x;
        if(N % p == 0) {
            return p;
        }
        A ++;
    }
    
    return -1;
}

string ex4(const mpz_class & N, int e_ui, const mpz_class & c) {
    mpz_class p = ex1(N);
    mpz_class q = N / p;
    mpz_class e(e_ui);
    assert(p*q == N);

    mpz_class totient = N - p - q + 1;
    mpz_class d;

    mpz_invert( d.get_mpz_t(), e.get_mpz_t(), totient.get_mpz_t() );
    mpz_class pt;

    mpz_powm ( pt.get_mpz_t(), c.get_mpz_t(), d.get_mpz_t(), N.get_mpz_t());
    string result = pt.get_str(16);
    string::size_type i = result.rfind("00");
    assert(i != string::npos);
//    cout << result.substr(i+2) << endl;

    unsigned char msb, lsb, uc;
    istringstream in( result.substr(i+2) );
    ostringstream out;
    while(in >> msb >> lsb) {
        uc = from_hex(msb, lsb);
        out << (char)uc;
    }
    
    return out.str();
}

void test_1() {
    mpz_class N("17976931348623159077293051907890247336179769789423065727343008115 \
    77326758055056206869853794492129829595855013875371640157101398586 \
    47833778606925583497541085196591615128057575940752635007475935288 \
    71082364994994077189561705436114947486504671101510156394068052754 \
    0071584560878577663743040086340742855278549092581");
    mpz_class R("13407807929942597099574024998205846127479365820592393377723561443 \
                             72176403007366276889111161436232699867504054609433932083841952337 \
                            5986027530441562135724301");

    assert(R == ex1(N));
}

void test_2() {
    mpz_class N("6484558428080716696628242653467722787263437207069762630604390703787 \
    9730861808111646271401527606141756919558732184025452065542490671989 \
    2428844841839353281972988531310511738648965962582821502504990264452 \
    1008852816733037111422964210278402893076574586452336833570778346897 \
    15838646088239640236866252211790085787877");
    
    mpz_class R("2546479614699618343800881656397394222934145426852415784632858192788 \
                            5777969985222835143851073249573454107384461557193173304497244814071 \
                            505790566593206419759");
    assert(R == ex2(N));
}

void test_4() {
   mpz_class N("17976931348623159077293051907890247336179769789423065727343008115 \
    77326758055056206869853794492129829595855013875371640157101398586 \
    47833778606925583497541085196591615128057575940752635007475935288 \
    71082364994994077189561705436114947486504671101510156394068052754 \
    0071584560878577663743040086340742855278549092581");
   mpz_class c("2209645186741038177630656113488341801741006978789283 \
1071731839143676135600120538004282329650473509424343946219751512256 \
4658399679428894607645420405815647489880137348641204523252293201764 \
8791666640299750918872997169052608322206777160001932926087000957999 \
3724077458967773697817571267229951148662959627934791540");
   
   assert("Factoring lets us break RSA." == ex4(N, 65537, c));
}

int main(int argc, char * argv[]) {
    test_1();
    test_2();
    test_4();
    
    return 0;
}
