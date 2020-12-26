/**
 * @file hash.c
 * @author Bilal Mohamad (bmohama)
 *
 * This is the main component. It contains the main function.
 * It's responsible for parsing the command-line arguments and using the other components
 * to read the input file, to perform the MD5 computation and to compute the HMAC if requested.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"
#include "buffer.h"
#include "hmac-md5.h"

/** Constant used for checking the number of arguments */
#define ARG_CHECK_3 3

/** Constant used for checking the number of arguments */
#define ARG_CHECK_4 4

/** Print out a usage message. */
static void usage() {
    fprintf(stderr, "usage: hash [-hmac <key>] <filename>\n");
}

/** Print out an incorrect file message */
static void usageFile(const char *filename) {
    fprintf(stderr, "Can't open file: %s\n", filename);
}

int main(int argc, char *argv[]) {

    if (argc <= 1 || argc == ARG_CHECK_3 || argc > ARG_CHECK_4) {
        usage();
        return EXIT_FAILURE;
    }

    Buffer *buff;
    if ((buff = readFile(argv[argc - 1])) == NULL) {
        usageFile(argv[argc - 1]);
        return EXIT_FAILURE;
    }

    MD5State state;
    initState(&state);
    unsigned char digest[MD5_DIGEST];

    if (argc == ARG_CHECK_4) {
        hmacMd5(argv[2], buff, digest);
//      printf("Key: %s\n", argv[2]);
    }
    else {
        padBuffer(buff);
        int check = buff->len - MD5_BLOCK;
        for (int i = 0; i <= check; i += MD5_BLOCK) {
            md5Block(&buff->data[i], &state);
        }

        md5Encode(digest, &state);
    }

    for (int i = 0; i < MD5_DIGEST; i++) {
        printf("%02X", digest[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;

}
