#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

#include <curl/curl.h>

#define DATA "f20bdba6ff29eed7b046d1df9fb7000058b1ffb4210a580f748b4ac714c001bd4a61044426fb515dad3f21f18aa577c0bdf302936266926ff37dbf7035d5eeb4"

unsigned char hc2b(char c) {
    switch(tolower(c)) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return c - '0';
        case 'a': case 'b': case 'c': case 'd': case 'e': 
        case 'f':
            return 10 + (tolower(c) - 'a');
        default:
            fprintf(stderr, "Invalid hex character: %c\n", c);
            exit(1);
    }
}

void from_hex(const char * in, unsigned char * out) {
    while(*in != 0) {
        unsigned char c = hc2b(*in++);
        c = (c << 4) | hc2b(*in++);
        *out++ = c;
    }
}

void to_hex(const unsigned char * in, const size_t l, char * out) {
    for(size_t i = 0; i < l; i++) {
        sprintf(out, "%02x", in[i]);
        out += 2;
    }
    *out = '\0';
}

static void test_hc2b() {
    assert(hc2b('0') == 0);
    assert(hc2b('1') == 1);
    assert(hc2b('2') == 2);
    assert(hc2b('3') == 3);
    assert(hc2b('4') == 4);
    assert(hc2b('5') == 5);
    assert(hc2b('6') == 6);
    assert(hc2b('7') == 7);
    assert(hc2b('8') == 8);
    assert(hc2b('9') == 9);
    assert(hc2b('A') == 10);
    assert(hc2b('b') == 11);
    assert(hc2b('C') == 12);
    assert(hc2b('d') == 13);
    assert(hc2b('e') == 14);
    assert(hc2b('f') == 0x0f);
}
static void test_from_hex() {
    unsigned char out[128];
    unsigned char exp[128] = {0xde, 0xad, 0xbe, 0xef};

    from_hex("deadbeef", out);
    assert(memcmp(exp, out, 4) == 0);
}

static void test_to_hex() {
    unsigned char out[128];
    char exp[128];

    memset(exp, 0, 128);
    from_hex("deadbeef", out);
    to_hex(out, 4, exp);
    assert(strcmp(exp, "deadbeef") == 0);
}

int request(CURL * curl, const char * url) {
    CURLcode res;
    long http_code;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        fprintf(stderr, "get failed %s\n", curl_easy_strerror(res));
        exit(1);
    }
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
    // we got 200 if message is correct (padding correct also !)
    return (http_code == 404L || http_code == 200L);
}

#define BLOCK_SIZE 16
int main(int argc, char * argv[]) {
    char          out[256];
    unsigned char data[64];
    char          result[64];
    size_t        size = 64;
    size_t        url_param;
    int bn = 3; // current block number
    CURL * curl;

#if 0
    test_hc2b();
    test_from_hex();
    test_to_hex();
#endif

    memset(out, 0, 256);
    memset(result, 0, 64);

    strcpy(out, "http://crypto-class.appspot.com/po?er=");
    url_param = strlen(out);
    curl = curl_easy_init();
    while(bn > 0) {
        from_hex(DATA, data);
        int pos = bn * BLOCK_SIZE; // end of current iv
        unsigned char pad = 0x01;
        for(int idx = 1; idx <= BLOCK_SIZE; idx++) {
            data[pos-idx] ^= pad;
            for(unsigned char c = 0x09; c < 255; c++) {
                data[pos-idx] ^= c;
                to_hex(data, (bn+1) * BLOCK_SIZE, out + url_param);
                if(request(curl, out)) {
                    fprintf(stderr, "%d: %d\n", pos-idx, c);
                    result[pos + BLOCK_SIZE - idx] = c;
                    break;
                }
                data[pos-idx] ^= c;
            }
            // increase the padding
            for(int j = idx; j > 0; j --) {
                data[pos-j] ^= (pad ^ (pad+1));
            }
            pad ++;
                
        }
        bn -= 1;
    }
    curl_easy_cleanup(curl);

    fprintf(stderr, "%s\n", result+BLOCK_SIZE);

    return 0;
}
