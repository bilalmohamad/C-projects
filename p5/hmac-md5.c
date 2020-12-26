/**
 * @file hmac-md5.c
 * @author Bilal Mohamad (bmohama)
 *
 * This component defines a single utility hmacMd5() for performing the HMAC operation for a
 * given key and input.
 */

#include <string.h>
#include "hmac-md5.h"
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
void hmacMd5(char *kstr, Buffer *b, unsigned char digest[MD5_DIGEST]) {

    unsigned char ipad[MD5_BLOCK];
    unsigned char opad[MD5_BLOCK];

    //XOR for the inner and outer pads
    for (int i = 0; i < MD5_BLOCK; i++) {
        if (i < strlen(kstr)) {
            ipad[i] = kstr[i] ^ 0x36;
            opad[i] = kstr[i] ^ 0x5c;
        }
        else {
            ipad[i] = 0x36;
            opad[i] = 0x5c;
        }
    }

    //Creates the buffers
    Buffer *ipadBuff = makeBuffer();
    Buffer *opadBuff = makeBuffer();

    //Appends to the buffers
    for (int i = 0; i < MD5_BLOCK; i++) {
        appendBuffer(ipadBuff, ipad[i]);
        appendBuffer(opadBuff, opad[i]);
    }

    //Pads the inner buffer
    for (int i = 0; i < b->len; i++) {
        appendBuffer(ipadBuff, b->data[i]);
    }
    padBuffer(ipadBuff);

    //Hashes the state of the inner pad
    unsigned char innerDigest[MD5_DIGEST];
    MD5State innerPad;
    initState(&innerPad);

    int innerCheck = ipadBuff->len - MD5_BLOCK;
    for (int i = 0; i <= innerCheck; i += MD5_BLOCK) {
        md5Block(&ipadBuff->data[i], &innerPad);
    }
    md5Encode(innerDigest, &innerPad);

    //Pads the outer buffer
    for (int i = 0; i < MD5_DIGEST; i++) {
        appendBuffer(opadBuff, innerDigest[i]);
    }
    padBuffer(opadBuff);

    //Hashes the state of the outer pad
    MD5State outerPad;
    initState(&outerPad);

    int outerCheck = opadBuff->len - MD5_BLOCK;
    for (int i = 0; i <= outerCheck; i += MD5_BLOCK) {
        md5Block(&opadBuff->data[i], &outerPad);
    }
    md5Encode(digest, &outerPad);

    //Frees the buffers
    freeBuffer(ipadBuff);
    freeBuffer(opadBuff);
}
