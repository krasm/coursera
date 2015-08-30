#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <openssl/sha.h>

#define MAX_SIZE (1024 + 32)

typedef struct node_st node_t;

struct node_st {
	uint8_t buffer[MAX_SIZE];
	size_t  data_len;
	node_t * next;
};

node_t * read_data(FILE * in) {
	uint8_t b[1024];
	int rn = 0;
	node_t* ret;

	if(feof(in) || ferror(in)) 
		return NULL;

	rn = fread(b, 1, 1024, in);
	fprintf(stderr, "read %d bytes\n", rn);
	ret = (node_t*)malloc(sizeof(node_t));
	ret->next = NULL;
	memcpy(ret->buffer, b, rn);
	ret->data_len = rn;

	return ret;
}

unsigned char * get_digest(node_t * node) {
	SHA256_CTX ctx;
	unsigned char * md = (unsigned char*)malloc(32);
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, node->buffer, node->data_len);
	SHA256_Final(md, &ctx);

	return md;
}

int main(int argc, char * argv[]) {
	
	FILE * in = fopen(argv[1], "rb");
	node_t * head = NULL;
	node_t * tmp;

	assert(in != NULL);

	while((tmp = read_data(in)) != NULL) {
		tmp->next = head;
		head = tmp;
	}
	fclose(in);

	{
		// calculate all hash sums 
		node_t * curr = head;
		unsigned char * md = NULL;
		while(curr != NULL) {
			if(md != NULL) {
				memcpy(curr->buffer+1024, md, 32);
				curr->data_len += 32;
				free(md);
			}
			md = get_digest(curr);
			curr = curr->next;
		}

		for(int i = 0; i < 32; i++) {
			printf("%02x", md[i]);
		}
		printf("\n");
	}
	return 0;
}
