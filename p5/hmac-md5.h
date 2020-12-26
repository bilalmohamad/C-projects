/**
 * @file hmac-md5.h
 * @author Bilal Mohamad (bmohama)
 *
 * This file acts as the header file for the hmac-md5.c file
 */

#include "md5.h"
#include "buffer.h"

/**
 * This function performs the HMAC-MD5.
 * It takes a key as a string of characters, a pointer to a Buffer struct,
 * and a pointer to an area of memory to store the digest (using  md5Encode()).
 * Note that you will likely not want the Buffer to have the padding, as the computation
 * requires you to put the ipad before the Buffer contents
 * (e.g., by making a new buffer and copying from the passed Buffer).
 *
 * @param kstr   the key as a string of characters
 * @param b      the buffer
 * @param digest the array containing the low and high order of the states
 */
void hmacMd5(char *kstr, Buffer *b, unsigned char digest[ MD5_DIGEST]);
