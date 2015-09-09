
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <gmp.h>

int main(int argc, char * argv[]) {
	unsigned long ts = atol(argv[1]);
	time_t sec = (time_t)ts / 1000;
	uint16_t msec = (ts % 1000);

 	struct tm * stm = gmtime(&sec);
 	struct tm * ltm = localtime(&sec);

	char buffer[1024];

	strftime(buffer, 1024, "%Y-%m-%d %H:%M:%S", stm);
	fprintf(stderr, "%s,%ld\n", buffer, msec); 

	strftime(buffer, 1024, "%Y-%m-%d %H:%M:%S", ltm);
	fprintf(stderr, "%s,%ld\n", buffer, msec); 

	return EXIT_SUCCESS;
}
