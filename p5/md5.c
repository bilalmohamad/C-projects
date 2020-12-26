/**
 * @file md5.c
 * @author Bilal Mohamad (bmohama)
 *
 * This component contains a struct to store the MD5 state and functions to compute
 * various parts of the MD5 algorithm.
 * The starter includes a partial implementation of this component, including a definition
 * of the MD5State struct.
 */

#include <stdio.h>
#include <stdlib.h>
#include "md5.h"

/** Constant used for rotating left */
#define LEFT 32

/** Constant used for loops of the encode */
#define LOW_ORDER_ENCODE 4

/** Constant used for the G function */
#define G3 3

/** Constant used for the G function */
#define G5 5

/** Constant used for the G function */
#define G7 7

/** Constant used for padding */
#define PAD_CONSTANT 55

/* Mechanism to conditionally expose static functions to other components.  For
 production, we can make make them static, but for testing we can disable
 the static keyword and expose functions to the test driver. */
#ifdef TESTABLE
#define test_static
#else
#define test_static static
#endif

/** Within each iteration, how many bits left do we rotate the a value? */
static int shift[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17,
        22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 4, 11, 16,
        23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10, 15, 21, 6, 10,
        15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

/** Constants, to mix in some random-looking bits during the MD5
 calculation.  These are computed from the sin() function.  They're
 examples of what might be called "Nothing-Up-My-Sleeve"
 numbers. */
static unsigned int noise[64] = { 0xd76aa478, 0xe8c7b756, 0x242070db,
        0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8,
        0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e,
        0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d,
        0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87,
        0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942,
        0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60,
        0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039,
        0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97, 0xab9423a7,
        0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 0x6fa87e4f,
        0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb,
        0xeb86d391 };

/**
 * Round 0 of the F function
 *
 * @param b the b state
 * @param c the c state
 * @param d the d state
 *
 * @return the result of the Round 0 F function
 */
test_static unsigned int fVersion0(unsigned int b, unsigned int c,
        unsigned int d) {
    return (b & c) | (~b & d);
}

/**
 * Round 1 of the F function
 *
 * @param b the b state
 * @param c the c state
 * @param d the d state
 *
 * @return the result of the Round 1 F function
 */
test_static unsigned int fVersion1(unsigned int b, unsigned int c,
        unsigned int d) {
    return (b & d) | (c & ~d);
}

/**
 * Round 2 of the F function
 *
 * @param b the b state
 * @param c the c state
 * @param d the d state
 *
 * @return the result of the Round 2 F function
 */
test_static unsigned int fVersion2(unsigned int b, unsigned int c,
        unsigned int d) {
    return b ^ c ^ d;
}

/**
 * Round 3 of the F function
 *
 * @param b the b state
 * @param c the c state
 * @param d the d state
 *
 * @return the result of the Round 3 F function
 */
test_static unsigned int fVersion3(unsigned int b, unsigned int c,
        unsigned int d) {
    return c ^ (b | ~d);
}

/**
 * Round 0 of the G function
 *
 * @param idx the index
 *
 * @return the result of the Round 0 G function
 */
test_static int gVersion0(int idx) {
    return idx;
}

/**
 * Round 1 of the G function
 *
 * @param idx the index
 *
 * @return the result of the Round 1 G function
 */
test_static int gVersion1(int idx) {
    return (G5 * idx + 1) % MD5_DIGEST;
}

/**
 * Round 2 of the G function
 *
 * @param idx the index
 *
 * @return the result of the Round 2 G function
 */
test_static int gVersion2(int idx) {
    return (G3 * idx + G5) % MD5_DIGEST;
}

/**
 * Round 3 of the G function
 *
 * @param idx the index
 *
 * @return the result of the Round 3 G function
 */
test_static int gVersion3(int idx) {
    return (G7 * idx) % MD5_DIGEST;
}

/**
 * Rotates the value left by s bits.
 *
 * @param value the value to be shifted
 * @param s     the number of bits to shift by
 *
 * @return the newly shifted value
 */
test_static unsigned int rotateLeft(unsigned int value, int s) {
    return value << s | value >> (LEFT - s);
}

test_static void md5Iteration(unsigned int data[MD5_DIGEST], unsigned int *a,
        unsigned int *b, unsigned int *c, unsigned int *d, int i) {

    unsigned int round = i / MD5_DIGEST;
    unsigned int result = 0;

    if (round == 0) {
        result = fVersion0(*b, *c, *d);
        result += *a;
        result += data[gVersion0(i)];
    }
    else if (round == 1) {
        result = fVersion1(*b, *c, *d);
        result += *a;
        result += data[gVersion1(i)];
    }
    else if (round == 2) {
        result = fVersion2(*b, *c, *d);
        result += *a;
        result += data[gVersion2(i)];
    }
    else {
        result = fVersion3(*b, *c, *d);
        result += *a;
        result += data[gVersion3(i)];
    }

    result += noise[i];
    result = rotateLeft(result, shift[i]);
    result += *b;

    *a = *d;
    *d = *c;
    *c = *b;
    *b = result;
}

/**
 * Given the address of an MD5State, this function initializes its fields, filling them in
 * with the four constant values given in the MD5 algorithm.
 *
 * @param state the state to initialize
 */
void initState(MD5State *state) {

    state->A = 0x67452301;
    state->B = 0xefcdab89;
    state->C = 0x98badcfe;
    state->D = 0x10325476;
}

/**
 * This function pads the given buffer, bringing its length up to a multiple of 64 bytes,
 * adding byte values as described int he MD5 algorithm.
 *
 * @param b the buffer being padded
 */
void padBuffer(Buffer *b) {

    unsigned long oldLen = b->len;
    appendBuffer(b, 0x80);

    unsigned long padding = (PAD_CONSTANT - oldLen) % MD5_BLOCK;
    for (int i = 0; i < padding; i++) {
        appendBuffer(b, 0);
    }

    oldLen *= 8;

    unsigned char *last = (unsigned char *) &oldLen;
    for (int i = 0; i < MD5_DIGEST / 2; i++) {
        appendBuffer(b, last[i]);
    }
}

/**
 * This function performs 64 MD5 iterations on the given block of bytes, updating the given state.
 *
 * @param data  the array containing the low and high order bytes of all the states
 * @param state the state to be encoded
 */
void md5Block(unsigned char data[MD5_BLOCK], MD5State *state) {

    unsigned int tempRange[MD5_DIGEST];

    for (int i = 0; i < MD5_DIGEST; i++) {
        tempRange[i] = *(unsigned int *) &data[LOW_ORDER_ENCODE * i];
    }

    //Store old states
    unsigned int oldA = state->A;
    unsigned int oldB = state->B;
    unsigned int oldC = state->C;
    unsigned int oldD = state->D;

    //Modify current states
    for (int i = 0; i < MD5_BLOCK; i++) {
        md5Iteration(tempRange, &state->A, &state->B, &state->C, &state->D, i);
    }

    //Add old states to the new modified states
    state->A += oldA;
    state->B += oldB;
    state->C += oldC;
    state->D += oldD;
}

/**
 * This function is used to create the final hash value (also known as a "digest").
 * It transfers the 16 bytes in the A, B, C, and D state variables into a
 * 16 byte unsigned char array.
 * The digest encodes A, then B, then C, then D, but the low-order bytes first.
 * That is digest[0] contains the low-order byte A, digest[3] contains the high-order byte of A,
 * digest[4] contains the low-order byte of B, and so on.
 *
 * @param digest    the array containing the low and high order of the states
 * @param state     the state to be encoded
 */
void md5Encode(unsigned char digest[MD5_DIGEST], MD5State *state) {

    int pos = 0;

    unsigned char *order = (unsigned char *) &state->A;
    for (int i = 0; i < LOW_ORDER_ENCODE; i++) {
        digest[pos] = order[i];
        pos++;
    }

    order = (unsigned char *) &state->B;
    for (int i = 0; i < LOW_ORDER_ENCODE; i++) {
        digest[pos] = order[i];
        pos++;
    }

    order = (unsigned char *) &state->C;
    for (int i = 0; i < LOW_ORDER_ENCODE; i++) {
        digest[pos] = order[i];
        pos++;
    }

    order = (unsigned char *) &state->D;
    for (int i = 0; i < LOW_ORDER_ENCODE; i++) {
        digest[pos] = order[i];
        pos++;
    }
}
