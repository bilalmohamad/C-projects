/**
 * @file md5.h
 * @author Bilal Mohamad (bmohama)
 *
 * This file acts as the header file for the md5.c file
 */

#ifndef _MD5_H_
#define _MD5_H_

#include "buffer.h"

/** Number of bytes in a block used int he MD5 calculation. */
#define MD5_BLOCK 64

/** Number of bytes in an MD5 digest */
#define MD5_DIGEST 16

/** Representation for the state of the MD5 computation.  It's just 4
 unsigned 32-bit integers. Client code can create an instance
 (statically, on the stack or on the heap), but initState() needs
 to initialize it before it can be used. */
typedef struct {
    /** Field A from the MD5 state.  This comment and the ones below
     aren't particularly meaningful, because there isn't really a
     particular meaning for each of these fields. */
    unsigned int A;

    /** Field B from the MD5 state.  */
    unsigned int B;

    /** Field B from the MD5 state.  */
    unsigned int C;

    /** Field B from the MD5 state.  */
    unsigned int D;
} MD5State;

/**
 * Given the address of an MD5State, this function initializes its fields, filling them in
 * with the four constant values given in the MD5 algorithm.
 *
 * @param state the state to initialize
 */
void initState(MD5State *state);

/**
 * This function pads the given buffer, bringing its length up to a multiple of 64 bytes,
 * adding byte values as described int he MD5 algorithm.
 *
 * @param b the buffer being padded
 */
void padBuffer(Buffer *b);

/**
 * This function performs 64 MD5 iterations on the given block of bytes, updating the given state.
 *
 * @param data  the array containing the low and high order bytes of all the states
 * @param state the state to be encoded
 */
void md5Block(unsigned char data[ MD5_BLOCK], MD5State *state);

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
void md5Encode(unsigned char digest[ MD5_DIGEST], MD5State *state);

#endif
