#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define KEY1 "140b41b22a29beb4061bda66b6747e14"
#define KEY1_LEN 32
#define CT1  "4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81"
#define CT1_LEN strlen(CT1)

unsigned char from_hex_char(unsigned char c) {
	switch(tolower(c)) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			return c - '0';
		case 'a':
			return 10;
		case 'b':
			return 11;
		case 'c':
			return 12;
		case 'd':
			return 13;
		case 'e':
			return 14;
		case 'f':
			return 15;
		default:
			fprintf(stderr, "Unknown digit %d\n", c);
			exit(1);
	}
}

unsigned char * from_hex(const char * in, size_t l) {
	unsigned char * out;
	unsigned char * ret;
	assert(in != NULL);
	assert(l > 0 && l % 2 == 0);
	ret = out = (unsigned char*)malloc(l / 2 + 1);

	while(*in) {
		unsigned char c = 0;
		c = from_hex_char(*in++);
		c = (c << 4) | from_hex_char(*in++);
		*out++ = c;
	}	
	*out = '\0';
	return ret;
}

char * to_hex(const unsigned char * in, size_t l) {
	char * out;
	char * ret;

	assert(in != NULL);
	assert(l > 0);
	ret = out = (char*)malloc(2*l+1);
	
	for(size_t i = 0; i < l; i++) {
		sprintf(out, "%02x", in[i]);
		out += 2;
	}
	*out = '\0';
	return ret;
}

static void test() {
	assert(0 == from_hex_char('0'));
	assert(1 == from_hex_char('1'));
	assert(2 == from_hex_char('2'));
	assert(3 == from_hex_char('3'));
	assert(4 == from_hex_char('4'));
	assert(5 == from_hex_char('5'));
	assert(6 == from_hex_char('6'));
	assert(7 == from_hex_char('7'));
	assert(8 == from_hex_char('8'));
	assert(9 == from_hex_char('9'));
	assert(10 == from_hex_char('a'));
	assert(11 == from_hex_char('B'));
	assert(12 == from_hex_char('c'));
	assert(13 == from_hex_char('d'));
	assert(14 == from_hex_char('e'));
	assert(15 == from_hex_char('F'));
	
	{
		unsigned char * t = from_hex(KEY1, KEY1_LEN);
		char * u = to_hex(t, KEY1_LEN / 2);
		assert(strcmp(KEY1, u) == 0);
		free(t); free(u);

		t = from_hex(CT1, CT1_LEN);
		u = to_hex(t, CT1_LEN / 2);
		assert(strcmp(CT1, u) == 0);
		free(t); free(u);
	}

}

int main(int argc, char * argv[]) {
	// some test 
	test();

	return EXIT_SUCCESS;
}
