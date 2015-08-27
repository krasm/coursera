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

#define K2 "140b41b22a29beb4061bda66b6747e14"
#define C2 "5b68629feb8606f9a6667670b75b38a5b4832d0f26e1ab7da33249de7d4afc48e713ac646ace36e872ad5fb8a512428a6e21364b0c374df45503473c5242a253"

#define K3 "36f18357be4dbd77f050515c73fcf9f2"
#define C3 "69dda8455c7dd4254bf353b773304eec0ec7702330098ce7f7520d1cbbb20fc388d1b0adb5054dbd7370849dbf0b88d393f252e764f1f5f7ad97ef79d59ce29f5f51eeca32eabedd9afa9329"

#define K4 "36f18357be4dbd77f050515c73fcf9f2"
#define C4 "770b80259ec33beb2561358a9f2dc617e46218c0a53cbeca695ae45faa8952aa0e311bde9d4e01726d3184c34451"

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

char * decrypt(const char * str_key, const char * str_ct) {
	EVP_CIPHER_CTX *ctx;
	size_t ctl = strlen(str_ct);
	size_t len = ctl / 2;
	size_t txt_len;
	char * out = (char*)malloc(len + 1);
	unsigned char * key = from_hex(str_key, strlen(str_key));
	unsigned char * ct = from_hex(str_ct, ctl);

	if( !(ctx = EVP_CIPHER_CTX_new()) ) {
		ERR_print_errors_fp(stderr);
		abort();
	}
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, ct)) {
		ERR_print_errors_fp(stderr);
		abort();
	}

	if(1 != EVP_DecryptUpdate(ctx, out, &len, ct+16, len - 16)) {
		ERR_print_errors_fp(stderr);
		abort();
	}

	txt_len = len;
	if(1 != EVP_DecryptFinal_ex(ctx, out + len, &len)) {
		ERR_print_errors_fp(stderr);
		abort();
	}
	txt_len += len;
	out[txt_len] = '\0';
	free(key);
	free(ct);

	/* Clean up */
    EVP_CIPHER_CTX_free(ctx);

	return out;
}

char * decrypt_1(const char * str_key, const char * str_ct) {
	EVP_CIPHER_CTX *ctx;
	size_t ctl = strlen(str_ct);
	size_t len = ctl / 2;
	size_t txt_len;
	char * out = (char*)malloc(len + 1);
	unsigned char * key = from_hex(str_key, strlen(str_key));
	unsigned char * ct = from_hex(str_ct, ctl);

	if( !(ctx = EVP_CIPHER_CTX_new()) ) {
		ERR_print_errors_fp(stderr);
		abort();
	}
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, ct)) {
		ERR_print_errors_fp(stderr);
		abort();
	}

	if(1 != EVP_DecryptUpdate(ctx, out, &len, ct+16, len - 16)) {
		ERR_print_errors_fp(stderr);
		abort();
	}

	txt_len = len;
	if(1 != EVP_DecryptFinal_ex(ctx, out + len, &len)) {
		ERR_print_errors_fp(stderr);
		abort();
	}
	txt_len += len;
	out[txt_len] = '\0';
	free(key);
	free(ct);

	/* Clean up */
    EVP_CIPHER_CTX_free(ctx);

	return out;
}

int main(int argc, char * argv[]) {
	// some test 
	test();
	char * result = decrypt(KEY1, CT1);
	printf("%s\n", result);
	result = decrypt(K2, C2);
	printf("%s\n", result);
	result = decrypt(K2, C2);
	printf("%s\n", result);
	result = decrypt_1(K3, C3);
	printf("%s\n", result);
	result = decrypt_1(K4, C4);
	printf("%s\n", result);
	return EXIT_SUCCESS;
}
